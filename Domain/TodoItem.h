#pragma once
#include "BaseEntity.h"
#include "TodoList.h"
#include <memory>

class TodoList;

class DOMAIN_API TodoItem : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(TodoItem, (BaseEntity), (), (TodoListId, Title, Note, IsCompleted), (todoList))
public:
	TodoItem();
	TodoItem(const TodoItem& todoItem);
	TodoItem(TodoItem&& todoItem) noexcept;
	TodoItem& operator=(const TodoItem& todoItem);
	TodoItem(const std::string& id, const std::string& todoListId, const std::string& title, const std::string& note, bool isCompleted, const std::string& createdDate = "", const std::string& modifedDate = "");
	TodoItem& operator=(TodoItem&& todoItem) noexcept;
	virtual std::string ToString();

	virtual std::string GetTodoListId()
	{
		return TodoListId;
	}
	virtual std::string GetTitle()
	{
		return Title;
	}
	virtual std::string GetNote()
	{
		return Note;
	}
	virtual bool GetIsCompleted()
	{
		return IsCompleted;
	}

protected:
	std::string TodoListId;
	std::string Title;
	std::string Note;
	bool IsCompleted = false;
public:
	std::shared_ptr<TodoList> todoList = nullptr;
};

