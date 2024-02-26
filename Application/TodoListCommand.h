#pragma once
#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "TodoList.h"

class APPLICATION_API TodoListCommand : public BaseCommand<TodoList>
{
public:
	TodoListCommand();
	//explicit TodoListCommand(SAConnection* con) : BaseCommand(con) {}
	~TodoListCommand();
};

