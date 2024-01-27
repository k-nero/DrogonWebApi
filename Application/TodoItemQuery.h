#pragma once
#include "BaseQuery.h"
#include "ITodoItemQuery.h"
#include "ApplicationApi.h"

class APPLICATION_API TodoItemQuery : public BaseQuery<TodoItem>, ITodoItemQuery
{
public:
	TodoItemQuery();
	explicit TodoItemQuery(SAConnection* con) : BaseQuery(con) {}
	~TodoItemQuery();
};