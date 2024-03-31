#include "pch.h"
#include "TodoItemService.h"

TodoItemService::TodoItemService() = default;

std::shared_ptr<TodoItem> TodoItemService::GetTodoItemById(const std::string& Id) noexcept(false)
{
	auto& ctx = RedisContext::GetInstance();
	Query<TodoItem> query;
	std::shared_ptr<std::string> json = nullptr;

	/*std::string table_name = std::string(typeid(TodoItem).name());
	std::string redis_key = table_name.substr(table_name.find_last_of(' ') + 1) + ":" + Id;*/

	std::string redis_key = "TodoItem:" + Id;

	if (!ctx.CreateSyncContext())
	{
		json = query.GetByIdEx(Id, { "TodoList" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json == nullptr)
		{
			 json = query.GetByIdEx(Id, { "TodoList" });
			 if (json == nullptr)
			 {
				 json = std::make_shared<std::string>("{}");
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

	return query.ParseFromJSON<std::shared_ptr<TodoItem>>(*json);
}

std::vector<std::shared_ptr<TodoItem>> TodoItemService::GetAllTodoItems() noexcept(false)
{
	auto& ctx = RedisContext::GetInstance();
	Query<TodoItem> query;
	std::shared_ptr<std::string> json = nullptr;

	std::string redis_key = "TodoList:TodoItem:";

	if (!ctx.CreateSyncContext())
	{
		json = query.GetAllEx("", { "TodoList" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json == nullptr)
		{
			json = query.GetAllEx("", { "TodoList" });
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
	return query.ParseFromJSON<std::vector<std::shared_ptr<TodoItem>>>(*json);
}


std::string TodoItemService::CreateTodoItem(TodoItemModel& todo_item_model) noexcept(false)
{
	BaseCommand<TodoItem> cmd;
	auto& ctx = RedisContext::GetInstance();
	ctx.CreateSyncContext();
	auto todo_item = Mapper::Map<TodoItemModel, TodoItem>(todo_item_model);
	todo_item.SetId(CoreHelper::CreateUUID());
	if (cmd.Create(todo_item) > 0)
	{
		ctx.RemoveKeyContaining("TodoItem:");
		return todo_item.GetId();
	}
	else
	{
		throw std::runtime_error("Failed to create todo item");
	}
}

int TodoItemService::UpdateTodoItem(TodoItemModel& todo_item_model, const std::string& Id) noexcept(false)
{
	auto& ctx = RedisContext::GetInstance();
	ctx.CreateSyncContext();
	BaseCommand<TodoItem> cmd;
	auto todo_item = Mapper::Map<TodoItemModel, TodoItem>(todo_item_model);
	if (cmd.Update(todo_item, EQ(Id)) > 0)
	{
		ctx.RemoveKeyContaining("TodoItem:");
		return 1;
	}
	else
	{
		throw std::runtime_error("TodoItem not found");
	}
}

int TodoItemService::DeleteTodoItem(const std::string& Id)
{
	auto& ctx = RedisContext::GetInstance();
	ctx.CreateSyncContext();
	BaseCommand<TodoItem> cmd;
	if (cmd.Delete(EQ(Id)) > 0)
	{
		ctx.RemoveKeyContaining("TodoItem:");
		return 1;
	}
	else
	{
		throw std::runtime_error("TodoItem not found");
	}
}

TodoItemService::~TodoItemService() = default;