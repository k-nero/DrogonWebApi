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

int TodoItemService::UpdateTodoItem(TodoItem* TodoItem, const std::string& Id) noexcept(false)
{
	TodoItemCommand cmd;
	return cmd.Update(TodoItem, EQ(Id));
}

int TodoItemService::DeleteTodoItem(const std::string& Id)
{
	TodoItemCommand cmd;
	return cmd.Delete(EQ(Id));
}

TodoItemService::~TodoItemService()
{

}