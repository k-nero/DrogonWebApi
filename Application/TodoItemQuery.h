#pragma once
#include "BaseQuery.h"
#include "ApplicationApi.h"

class APPLICATION_API TodoItemQuery : public BaseQuery<TodoItem>
{
public:
	TodoItemQuery();
	//explicit TodoItemQuery(SAConnection* con) : BaseQuery(con) {}
	~TodoItemQuery();
};