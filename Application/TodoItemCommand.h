#pragma once
#include "TodoItem.h"
#include <SQLAPI.h>
#include <vector>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "ITodoItemCommand.h"
#include "BaseCommand.h"


class APPLICATION_API TodoItemCommand : public BaseCommand<TodoItem>, ITodoItemCommand
{
public:
	TodoItemCommand();
	//explicit TodoItemCommand(SAConnection* con) : BaseCommand(con) {}
	~TodoItemCommand();
};

