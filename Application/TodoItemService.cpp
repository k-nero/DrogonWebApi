#include "pch.h"
#include "TodoItemService.h"
#include "DbContext.h"
#include "TodoListQuery.h"
#include "RedisContext.h"

TodoItemService::TodoItemService()
{

}

std::shared_ptr<TodoItem> TodoItemService::GetTodoItemById(const std::string& Id) noexcept(false)
{
	/*TodoItemQuery todo_item_query;
	return todo_item_query.GetSingle(EQ(Id), { "TodoList" });*/
	RedisContext ctx;
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
		if (json->empty())
		{
			auto json = query.GetByIdEx(Id, { "TodoList" });
			ctx.SetString(redis_key, *json, 360);
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
	/*TodoItemQuery query;
	return query.GetAll();*/
	RedisContext ctx;
	Query<TodoItem> query;
	std::shared_ptr<std::string> json = nullptr;

	std::string redis_key = "TodoItem:";

	if (!ctx.CreateSyncContext())
	{
		json = query.GetAllEx("", { "TodoList" });
	}
	else
	{
		json = ctx.GetString(redis_key);
		if (json->empty())
		{
			auto json = query.GetAllEx("", { "TodoList" });
			ctx.SetString(redis_key, *json, 360);
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
	TodoItemCommand cmd;
	auto todo_item = Mapper::Map<TodoItemModel, TodoItem>(todo_item_model);
	todo_item.SetId(CoreHelper::CreateUUID());
	cmd.Create(todo_item);
	return todo_item.GetId();
}

int TodoItemService::UpdateTodoItem(TodoItemModel& todo_item_model, const std::string& Id) noexcept(false)
{
	TodoItemCommand cmd;
	auto todo_item = Mapper::Map<TodoItemModel, TodoItem>(todo_item_model);
	return cmd.Update(todo_item, EQ(Id));
}

int TodoItemService::DeleteTodoItem(const std::string& Id)
{
	TodoItemCommand cmd;
	return cmd.Delete(EQ(Id));
}

TodoItemService::~TodoItemService()
{

}