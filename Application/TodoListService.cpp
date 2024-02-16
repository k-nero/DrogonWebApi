#include "pch.h"
#include "TodoListService.h"
#include "DbContext.h"
#include "TodoItemQuery.h"

TodoListService::TodoListService()
{

}

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& Id) noexcept(false)
{
	TodoListQuery todo_list_query;
	return todo_list_query.GetSingle(EQ(Id), { "TodoItems" });
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() noexcept(false)
{
	TodoListQuery query;
	return query.GetAll();
}


int TodoListService::CreateTodoList(TodoList* TodoList) noexcept(false)
{
	throw std::exception("Not implemented");
	return 0;
}

int TodoListService::UpdateTodoList(TodoList* TodoList) noexcept(false)
{
	throw std::exception("Not implemented");

	return 0;
}

TodoListService::~TodoListService()
{

}

