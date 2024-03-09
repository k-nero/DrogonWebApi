#pragma once
#include <TodoList.h>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "TodoListCommand.h"
#include "TodoListQuery.h"
#include "Mapper.h"
#include "TodoListModel.h"
#include "PaginationObject.h"
#include "RedisContext.h"

class APPLICATION_API TodoListService
{
public:
	TodoListService();
	std::shared_ptr<TodoList> GetTodoListById(const std::string& id);
	std::vector<std::shared_ptr<TodoList>>GetAllTodoLists();
	std::shared_ptr<PaginationObject<TodoList>> GetTodoListsByPage(int page, int page_size);
	std::string CreateTodoList(TodoListModel& todo_list_model);
	int UpdateTodoList(TodoListModel& todo_list_model, const std::string& id);
	int DeleteTodoList(const std::string& id);
	~TodoListService();
};

