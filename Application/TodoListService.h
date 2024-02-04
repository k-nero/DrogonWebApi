#pragma once
#include <TodoList.h>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "TodoListCommand.h"
#include "TodoListQuery.h"


class APPLICATION_API TodoListService
{
public:
	TodoListService();
	//explicit TodoListService(SAConnection* con) { this->con = con; }
	std::shared_ptr<TodoList> GetTodoListById(const std::string& id);
	std::vector<std::shared_ptr<TodoList>>GetAllTodoLists();
	int CreateTodoList(TodoList* TodoList);
	int UpdateTodoList(TodoList* TodoList);
	~TodoListService();
private:
	//SAConnection* con;
};
