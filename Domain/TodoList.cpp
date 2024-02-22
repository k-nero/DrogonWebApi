#include "pch.h"
#include "TodoList.h"

TodoList::TodoList() : BaseEntity()
{
}

TodoList::TodoList(const TodoList& todoList) : BaseEntity(todoList)
{
	Title = todoList.Title;
	Description = todoList.Description;
	TodoItems = todoList.TodoItems;
}

TodoList::TodoList(TodoList&& todoList) noexcept : BaseEntity(todoList)
{
	Title = std::move(todoList.Title);
	Description = std::move(todoList.Description);
	TodoItems = std::move(todoList.TodoItems);
}

TodoList& TodoList::operator=(const TodoList& todoList) = default;

TodoList::TodoList(const std::string& id, const std::string& title, const std::string& description, const std::string& createdDate, const std::string& modifedDate) : BaseEntity(id, createdDate, modifedDate)
{
	Title = title;
	Description = description;
}

TodoList::TodoList( const std::string& title, const std::string& description, const std::string& createdDate, const std::string& modifedDate) 
{
	Title = title;
	Description = description;
	CreatedDate = createdDate;
	ModifiedDate = modifedDate;
}

TodoList& TodoList::operator=(TodoList&& todoList) noexcept = default;

std::string TodoList::ToString()
{
	return std::string();
}
