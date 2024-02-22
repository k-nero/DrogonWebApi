#include "pch.h"
#include "TodoItemService.h"
#include "DbContext.h"
#include "TodoListQuery.h"

TodoItemService::TodoItemService()
{

}

std::shared_ptr<TodoItem> TodoItemService::GetTodoItemById(const std::string& Id) noexcept(false)
{
	TodoItemQuery todo_item_query;
	return todo_item_query.GetSingle(EQ(Id), { "TodoList" });
}

std::vector<std::shared_ptr<TodoItem>> TodoItemService::GetAllTodoItems() noexcept(false)
{
	TodoItemQuery query;
	return query.GetAll();
}


std::string TodoItemService::CreateTodoItem(TodoItem* TodoItem) noexcept(false)
{
	TodoItemCommand cmd;
	TodoItem->SetId(CoreHelper::CreateUUID());
	cmd.Create(TodoItem);
	return TodoItem->GetId();
}

int TodoItemService::UpdateTodoItem(TodoItem* TodoItem) noexcept(false)
{
	throw std::exception("Not implemented");

	return 0;
}

TodoItemService::~TodoItemService()
{

}