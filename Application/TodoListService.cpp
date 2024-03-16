#include "pch.h"
#include "TodoListService.h"

TodoListService::TodoListService()
{

}

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& Id) noexcept(false)
{
	RedisContext ctx;
	ctx.CreateSyncContext();
	std::shared_ptr<std::string> json = nullptr;
	Query<TodoList> todo_list_query;
	std::string redis_key = "TodoList:TodoItem:" + Id;

	if (!ctx.CreateSyncContext())
	{
		json = todo_list_query.GetByIdEx(Id, { "TodoItems" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json->empty())
		{
			json = todo_list_query.GetByIdEx(Id, { "TodoItems" });
			if (json->empty())
			{
				throw NotFoundExcept();
			}
			ctx.SetString(redis_key, *json, 360);
		}
		else
		{
			ctx.refreshTTL(redis_key, 360);
		}
	}
	return todo_list_query.ParseFromJSON<std::shared_ptr<TodoList>>(*json);
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() noexcept(false)
{
	Query<TodoList> query;
	RedisContext ctx;
	std::shared_ptr<std::string> json = nullptr;
	std::string redis_key = "TodoList:TodoItem:";
	if (!ctx.CreateSyncContext())
	{
		json = query.GetAllEx({ "TodoItems" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json->empty())
		{
			json = query.GetAllEx({ "TodoItems" });
			ctx.SetString(redis_key, *json, 360);
		}
		else
		{
			ctx.refreshTTL(redis_key, 360);
		}
	}
	return query.ParseFromJSON<std::vector<std::shared_ptr<TodoList>>>(*json);
}

std::shared_ptr<PaginationObject<TodoList>> TodoListService::GetTodoListsByPage(int page, int page_size)
{
	Query<TodoList> query;
	RedisContext ctx;
	std::shared_ptr<std::string> json = nullptr;
	std::string redis_key = "TodoList:TodoItem:page=" + std::to_string(page) + "&limit=" + std::to_string(page_size);

	if (!ctx.CreateSyncContext())
	{
		json = query.GetPaginatedFw(page, page_size, "", { "TodoItems" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json->empty())
		{
			json = query.GetPaginatedFw(page, page_size, "", {"TodoItems"});
			ctx.SetString(redis_key, *json, 360);
		}
		else
		{
			ctx.refreshTTL(redis_key, 360);
		}
	}

	return query.ParseFromJSON<std::shared_ptr<PaginationObject<TodoList>>>(*json);
}


std::string TodoListService::CreateTodoList(TodoListModel& todo_list_model) noexcept(false)
{
	TodoListCommand cmd;
	RedisContext ctx;
	ctx.CreateSyncContext();
	auto todo_list = Mapper::Map<TodoListModel, TodoList>(todo_list_model);
	todo_list.SetId(CoreHelper::CreateUUID());
	if (cmd.Create(todo_list) > 0)
	{
		auto keys = ctx.GetAllActiveKeys();
		for (auto& key : keys)
		{
			if (key.find("TodoList:") != std::string::npos)
			{
				ctx.RemoveKey(key);
			}
		}

		return todo_list.GetId();
	}
	else
	{
		throw std::runtime_error("Create TodoList failed");
	}
}

int TodoListService::UpdateTodoList(TodoListModel& todo_list_model, const std::string& Id) noexcept(false)
{
	TodoListCommand cmd;
	RedisContext ctx;
	ctx.CreateSyncContext();
	auto todo_list = Mapper::Map<TodoListModel, TodoList>(todo_list_model);
	if (cmd.Update(todo_list, EQ(Id)) > 0)
	{
		auto keys = ctx.GetAllActiveKeys();
		for (auto& key : keys)
		{
			if (key.find("TodoList:") != std::string::npos)
			{
				ctx.RemoveKey(key);
			}
		}
		return 1;
	}
	else
	{
		throw std::runtime_error("Update TodoList failed");
	}
}

int TodoListService::DeleteTodoList(const std::string& Id)
{
	TodoListCommand cmd;
	RedisContext ctx;
	ctx.CreateSyncContext();
	if (cmd.Delete(EQ(Id)) > 0)
	{
		auto keys = ctx.GetAllActiveKeys();
		for (auto& key : keys)
		{
			if (key.find("TodoList:") != std::string::npos)
			{
				ctx.RemoveKey(key);
			}
		}
		return 1;
	}
	else
	{
		throw std::runtime_error("Delete TodoList failed");
	}
}

TodoListService::~TodoListService()
{

}

