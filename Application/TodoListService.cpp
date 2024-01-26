#include "pch.h"
#include "TodoListService.h"
#include "DbContext.h"

TodoListService::TodoListService()
{

}

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& id) throw (std::exception)
{
	try
	{
		DbContext db;
		TodoListQuery query(db.GetConnection());
		return query.GetById(id);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return nullptr;
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() throw (std::exception)
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


int TodoListService::CreateTodoList(TodoList* TodoList) throw (std::exception)
{
	/*try
	{
		DbContext db;
		TodoListCommand cmd(db.GetConnection());
		return cmd.Create(TodoList);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}*/
	return 0;
}

int TodoListService::UpdateTodoList(TodoList* TodoList) throw (std::exception)
{
	/*try
	{
		DbContext db;
		TodoListCommand cmd(db.GetConnection());
		return cmd.Update(TodoList);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}*/
	return 0;
}

TodoListService::~TodoListService()
{

}

