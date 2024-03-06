#include "pch.h"
#include "TodoItemService.h"
#include "DbContext.h"
#include "TodoListQuery.h"

TodoItemService::TodoItemService()
{

}

std::shared_ptr<TodoItem> TodoItemService::GetTodoItemById(const std::string& Id) noexcept(false)
{
	/*TodoItemQuery todo_item_query;
	return todo_item_query.GetSingle(EQ(Id), { "TodoList" });*/
	Query<TodoItem> query;
	return query.GetByIdEw(Id, { "TodoList" });
}

std::vector<std::shared_ptr<TodoItem>> TodoItemService::GetAllTodoItems() noexcept(false)
{
	/*TodoItemQuery query;
	return query.GetAll();*/
	Query<TodoItem> query;
	return query.GetAllEw("", {"TodoList"});
}


std::string TodoItemService::CreateTodoItem(TodoItemModel& todo_item_model) noexcept(false)
{
	TodoItemCommand cmd;
	auto todo_item = Mapper::Map<TodoItemModel, TodoItem>(todo_item_model);
	todo_item.SetId(CoreHelper::CreateUUID());
	cmd.Create(todo_item);
	return todo_item.GetId();
}

int TodoItemService::UpdateTodoItem(TodoItemModel& todo_item_model, const std::string& Id) noexcept(false)
{
	TodoItemCommand cmd;
	auto todo_item = Mapper::Map<TodoItemModel, TodoItem>(todo_item_model);
	return cmd.Update(todo_item, EQ(Id));
}

int TodoItemService::DeleteTodoItem(const std::string& Id)
{
	TodoItemCommand cmd;
	return cmd.Delete(EQ(Id));
}

TodoItemService::~TodoItemService()
{

}