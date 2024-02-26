#include "pch.h"
#include "TodoListService.h"

TodoListService::TodoListService()
{

}

std::shared_ptr<TodoList> TodoListService::GetTodoListById(const std::string& Id) noexcept(false)
{
	TodoListQuery todo_list_query;
	return todo_list_query.GetSingle(EQ(Id), {"TodoItems"});
}

std::vector<std::shared_ptr<TodoList>> TodoListService::GetAllTodoLists() noexcept(false)
{
	TodoListQuery query;
	return query.GetAll();
}


std::string TodoListService::CreateTodoList(TodoListModel& todo_list_model) noexcept(false)
{
	TodoListCommand cmd;
	auto todo_list = Mapper::Map<TodoListModel, TodoList>(todo_list_model);
	todo_list.SetId(CoreHelper::CreateUUID());
	cmd.Create(todo_list);
	return todo_list.GetId();
}

int TodoListService::UpdateTodoList(TodoListModel& todo_list_model, const std::string& Id) noexcept(false)
{
	TodoListCommand cmd;
	auto todo_list = Mapper::Map<TodoListModel, TodoList>(todo_list_model);
	return cmd.Update(todo_list, EQ(Id));

}

int TodoListService::DeleteTodoList(const std::string& Id)
{
	TodoListCommand cmd;
	return cmd.Delete(EQ(Id));
}

TodoListService::~TodoListService()
{

}

