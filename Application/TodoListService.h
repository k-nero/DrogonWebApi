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
	std::shared_ptr<TodoList> GetTodoListById(const std::string& id);
	std::vector<std::shared_ptr<TodoList>>GetAllTodoLists();
	std::string CreateTodoList(TodoList* TodoList);
	int UpdateTodoList(TodoList* TodoList);
	~TodoListService();
};

