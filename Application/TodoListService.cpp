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
	return todo_list_query.GetSingle(EQ(Id), {"TodoItems"});
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() noexcept(false)
{
	TodoListQuery query;
	return query.GetAll();
}


std::string TodoListService::CreateTodoList(TodoList& todo_list) noexcept(false)
{
	throw std::exception("Not implemented");
}

int TodoListService::UpdateTodoList(TodoList& todo_list) noexcept(false)
{
	throw std::exception("Not implemented");

	return 0;
}

TodoListService::~TodoListService()
{

}

