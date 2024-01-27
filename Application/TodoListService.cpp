#include "pch.h"
#include "TodoListService.h"
#include "DbContext.h"
#include "TodoItemQuery.h"

TodoListService::TodoListService()
{

}

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& id) throw (std::exception&)
{
	try
	{
		DbContext db;
		auto con = db.GetConnection();
		TodoListQuery todo_list_query(con);
		TodoItemQuery todo_item_query(con);

		auto todo_list = todo_list_query.GetById(id);
		todo_list.get()->SetTodoItems(todo_item_query.GetAll("TodoListId = '" + id + "'"));

		return todo_list;
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return nullptr;
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() throw (std::exception&)
{
	std::vector<std::shared_ptr<TodoList>> TodoLists;
	try
	{
		DbContext db;
		TodoListQuery query(db.GetConnection());
		TodoLists = query.GetAll();
		return TodoLists;
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return TodoLists;
}


int TodoListService::CreateTodoList(TodoList* TodoList) throw (std::exception&)
{
	throw std::exception("Not implemented");
	return 0;
}

int TodoListService::UpdateTodoList(TodoList* TodoList) throw (std::exception&)
{
	throw std::exception("Not implemented");

	return 0;
}

TodoListService::~TodoListService()
{

}

