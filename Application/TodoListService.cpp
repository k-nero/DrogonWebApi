#include "pch.h"
#include "TodoListService.h"
#include "DbContext.h"
#include "TodoItemQuery.h"

TodoListService::TodoListService()
{

}

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& Id) noexcept(false)
{
	DbContext db;
	auto con = db.GetConnection();
	TodoListQuery todo_list_query(con);
	auto todo_list = todo_list_query.GetSingle(EQ(Id), {"TodoItems"});
	return todo_list;
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() noexcept(false)
{
	std::vector<std::shared_ptr<TodoList>> TodoLists;
	DbContext db;
	TodoListQuery query(db.GetConnection());
	TodoLists = query.GetAll();
	return TodoLists;
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

