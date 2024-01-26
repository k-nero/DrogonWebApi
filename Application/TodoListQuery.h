#pragma once
#include "BaseQuery.h"
#include "ITodoListQuery.h"
#include "ApplicationApi.h"

class APPLICATION_API TodoListQuery : public BaseQuery<TodoList>, ITodoListQuery
{
public:
	TodoListQuery();
	explicit TodoListQuery(SAConnection* con) : BaseQuery(con) {}
	~TodoListQuery();
};