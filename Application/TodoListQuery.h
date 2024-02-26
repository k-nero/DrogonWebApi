#pragma once
#include "BaseQuery.h"
#include "ApplicationApi.h"

class APPLICATION_API TodoListQuery : public BaseQuery<TodoList>
{
public:
	TodoListQuery();
	//explicit TodoListQuery(SAConnection* con) : BaseQuery(con) {}
	~TodoListQuery();
};