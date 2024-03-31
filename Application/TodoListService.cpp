#include "pch.h"
#include "TodoListService.h"

TodoListService::TodoListService() = default;

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& Id) noexcept(false)
{
	auto& ctx = RedisContext::GetInstance();
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
		if (json == nullptr)
		{
			json = todo_list_query.GetByIdEx(Id, { "TodoItems" });
			if (json == nullptr)
			{
				return nullptr;
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
	auto& ctx = RedisContext::GetInstance();
	std::shared_ptr<std::string> json = nullptr;
	std::string redis_key = "TodoList:TodoItem:";
	if (!ctx.CreateSyncContext())
	{
		json = query.GetAllEx({ "TodoItems" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json == nullptr)
		{
			json = query.GetAllEx({ "TodoItems" });
			if (json == nullptr)
			{
				json = std::make_shared<std::string>("[]");
			}
			else
			{
				ctx.SetString(redis_key, *json, 360);
			}
		}
		else
		{
			ctx.refreshTTL(redis_key, 360);
		}
	}
	return query.ParseFromJSON<std::vector<std::shared_ptr<TodoList>>>(*json);
}

std::shared_ptr<PaginationObject<TodoList>> TodoListService::GetTodoListsByPage(int page, int page_size) noexcept(false)
{
	Query<TodoList> query;
	auto& ctx = RedisContext::GetInstance();
	std::shared_ptr<std::string> json = nullptr;
	std::string redis_key = "TodoList:TodoItem:page=" + std::to_string(page) + "&limit=" + std::to_string(page_size);

	if (!ctx.CreateSyncContext())
	{
		json = query.GetPaginatedFw(page, page_size, "", { "TodoItems" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json == nullptr)
		{
			json = query.GetPaginatedFw(page, page_size, "", { "TodoItems" });

			if (json == nullptr)
			{
				return nullptr;
			}

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
	BaseCommand<TodoList> cmd;
	auto& ctx = RedisContext::GetInstance();
	ctx.CreateSyncContext();
	auto todo_list = Mapper::Map<TodoListModel, TodoList>(todo_list_model);
	todo_list.SetId(CoreHelper::CreateUUID());
	if (cmd.Create(todo_list) > 0)
	{
		ctx.RemoveKeyContaining("TodoList:");
		return todo_list.GetId();
	}
	else
	{
		throw std::runtime_error("Create TodoList failed");
	}
}

int TodoListService::UpdateTodoList(TodoListModel& todo_list_model, const std::string& Id) noexcept(false)
{
	BaseCommand<TodoList> cmd;
	auto& ctx = RedisContext::GetInstance();
	ctx.CreateSyncContext();
	auto todo_list = Mapper::Map<TodoListModel, TodoList>(todo_list_model);
	if (cmd.Update(todo_list, EQ(Id)) > 0)
	{
		ctx.RemoveKeyContaining("TodoList:");
		return 1;
	}
	else
	{
		throw std::runtime_error("Update TodoList failed");
	}
}

int TodoListService::DeleteTodoList(const std::string& Id) noexcept(false)
{
	BaseCommand<TodoList> cmd;
	auto& ctx = RedisContext::GetInstance();
	ctx.CreateSyncContext();
	if (cmd.Delete(EQ(Id)) > 0)
	{
		ctx.RemoveKeyContaining("TodoList:");
		return 1;
	}
	else
	{
		throw std::runtime_error("Delete TodoList failed");
	}
}

TodoListService::~TodoListService() = default;

