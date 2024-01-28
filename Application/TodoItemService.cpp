#include "pch.h"
#include "TodoItemService.h"
#include "DbContext.h"
#include "TodoListQuery.h"

TodoItemService::TodoItemService()
{

}

std::shared_ptr<TodoItem> TodoItemService::GetTodoItemById(const std::string& id) noexcept(false)
{
	DbContext db;
	auto con = db.GetConnection();
	TodoItemQuery todo_item_query(con);
	TodoListQuery todo_list_query(con);
	auto todo_item = todo_item_query.GetById(id);
	todo_item.get()->SetTodoList(todo_list_query.GetById(todo_item.get()->GetTodoListId()));
	return todo_item;
}

std::vector<std::shared_ptr<TodoItem>> TodoItemService::GetAllTodoItems() noexcept(false)
{
	std::vector<std::shared_ptr<TodoItem>> TodoItems;
	DbContext db;
	TodoItemQuery query(db.GetConnection());
	TodoItems = query.GetAll();
	return TodoItems;
}


int TodoItemService::CreateTodoItem(TodoItem* TodoItem) noexcept(false)
{
	throw std::exception("Not implemented");
	return 0;
}

int TodoItemService::UpdateTodoItem(TodoItem* TodoItem) noexcept(false)
{
	throw std::exception("Not implemented");

	return 0;
}

TodoItemService::~TodoItemService()
{

}

