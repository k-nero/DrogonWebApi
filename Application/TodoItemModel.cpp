#include "pch.h"
#include "TodoItemModel.h"

TodoItemModel::TodoItemModel() = default;

TodoItemModel::TodoItemModel(std::string TodoListId, std::string Title, std::string Note, bool IsCompleted)
{
	this->TodoListId = TodoListId;
	this->Title = Title;
	this->Note = Note;
	this->IsCompleted = IsCompleted;
}

TodoItemModel::TodoItemModel(TodoItemModel& todo_item_model)
{
	this->TodoListId = todo_item_model.TodoListId;
	this->IsCompleted = todo_item_model.IsCompleted;
	this->Title = todo_item_model.Title;
	this->Note = todo_item_model.Note;
}

TodoItemModel& TodoItemModel::operator=(const TodoItemModel& todo_item_model) = default;

TodoItemModel& TodoItemModel::operator=(TodoItemModel&& todo_item_model) noexcept = default;

TodoItemModel::TodoItemModel(TodoItemModel&& todo_item_model) noexcept
{
	this->TodoListId = std::move(todo_item_model.TodoListId);
	this->IsCompleted = std::move(todo_item_model.IsCompleted);
	this->Title = std::move(todo_item_model.Title);
	this->Note = std::move(todo_item_model.Note);
}

TodoItemModel::~TodoItemModel() = default;
