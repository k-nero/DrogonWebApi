#pragma once
#include "BaseEntity.h"
#include "vector"
#include "TodoItem.h"
#include "memory"

class TodoItem;

class DOMAIN_API TodoList : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(TodoList, (BaseEntity), (), (Title, Description), ())

public:
	TodoList();
	TodoList(const TodoList& todoList);
	TodoList(TodoList&& todoList) noexcept;
	TodoList& operator=(const TodoList& todoList);
	TodoList(const std::string& id, const std::string& title, const std::string& description, const std::string& createdDate = "", const std::string& modifedDate = "");
	TodoList& operator=(TodoList&& todoList) noexcept;
	virtual std::string ToString();

	virtual std::string GetTitle()
	{
		return Title;
	}
	virtual std::string GetDescription()
	{
		return Description;
	}
	virtual void SetTitle(const std::string& title)
	{
		Title = title;
	}
	virtual void SetDescription(const std::string& description)
	{
		Description = description;
	}
	virtual std::vector<std::shared_ptr<TodoItem>> GetTodoItems()
	{
		return TodoItems;
	}
	virtual void SetTodoItems(std::vector<std::shared_ptr<TodoItem>> todoItems)
	{
		TodoItems = todoItems;
	}
protected:
	std::string Title = {};
	std::string Description = {};
	std::vector<std::shared_ptr<TodoItem>> TodoItems = {};
};

