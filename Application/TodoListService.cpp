#include "pch.h"
#include "TodoListService.h"
#include "DbContext.h"
#include "TodoItemQuery.h"

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


std::string TodoListService::CreateTodoList(TodoList* todo_list) noexcept(false)
{
	TodoListCommand cmd;
	todo_list->SetId(CoreHelper::CreateUUID());
	cmd.Create(todo_list);
	return todo_list->GetId();
}

int TodoListService::UpdateTodoList(TodoList* todo_list, const std::string& id) noexcept(false)
{
	TodoListCommand cmd;
	return cmd.Update(todo_list, id);

}

int TodoListService::DeleteTodoList(const std::string& id)
{
	TodoListCommand cmd;
	return cmd.Delete(id);
}

TodoListService::~TodoListService()
{

}

