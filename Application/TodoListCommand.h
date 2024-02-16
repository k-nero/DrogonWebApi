#pragma once
#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "TodoList.h"
#include "ITodoListCommand.h"

class APPLICATION_API TodoListCommand : public BaseCommand<TodoList>, ITodoListCommand
{
public:
	TodoListCommand();
	//explicit TodoListCommand(SAConnection* con) : BaseCommand(con) {}
	~TodoListCommand();
};

