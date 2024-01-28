#include "pch.h"
#include "TodoItem.h"

TodoItem::TodoItem() : BaseEntity()
{

}

TodoItem::TodoItem(const TodoItem& todoItem) : BaseEntity(todoItem)
{
	Id = todoItem.Id;
	TodoListId = todoItem.TodoListId;
	Title = todoItem.Title;
	Note = todoItem.Note;
	todoList = todoItem.todoList;
}

TodoItem::TodoItem(TodoItem&& todoItem) noexcept : BaseEntity(todoItem)
{
	Id = std::move(todoItem.Id);
	TodoListId = std::move(todoItem.TodoListId);
	Title = std::move(todoItem.Title);
	Note = std::move(todoItem.Note);
	IsCompleted = todoItem.IsCompleted;
	todoList = std::move(todoItem.todoList);
}

TodoItem& TodoItem::operator=(const TodoItem& todoItem) = default;

TodoItem::TodoItem(const std::string& id, const std::string& todoListId, const std::string& title, const std::string& note, bool isCompleted, const std::string& createdDate, const std::string& modifedDate) : BaseEntity(id, createdDate, modifedDate)
{
	TodoListId = todoListId;
	Title = title;
	Note = note;
	IsCompleted = isCompleted;
}

TodoItem& TodoItem::operator=(TodoItem&& todoItem) noexcept = default;

std::string TodoItem::ToString()
{
	return std::string();
}

TodoItem::~TodoItem()
{
}
