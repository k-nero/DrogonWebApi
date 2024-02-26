#pragma once
#include <string>
#include "ApplicationApi.h"
#include <boost/describe.hpp>


class APPLICATION_API TodoItemModel
{

	BOOST_DESCRIBE_CLASS(TodoItemModel, (), (), (), (TodoListId, Title, Note, IsCompleted))
public:
	TodoItemModel();
	TodoItemModel(std::string TodoListId, std::string Title, std::string Note, bool IsCompleted);
	TodoItemModel(TodoItemModel& todo_item_model);
	TodoItemModel& operator=(const TodoItemModel& todo_item_model);
	TodoItemModel& operator=(TodoItemModel&& todo_item_model) noexcept;
	TodoItemModel(TodoItemModel&& todo_item_model) noexcept;
	~TodoItemModel();

	std::string GetTodoListId() const
	{
		return TodoListId;
	}
	void SetTodoListId(const std::string& TodoListId)
	{
		this->TodoListId = TodoListId;
	}
	std::string GetTitle() const
	{
		return Title;
	}
	void SetTitle(const std::string& Title)
	{
		this->Title = Title;
	}
	std::string GetNote() const
	{
		return Note;
	}
	void SetNote(const std::string& Note)
	{
		this->Note = Note;
	}
	bool GetIsCompleted() const
	{
		return IsCompleted;
	}
	void SetIsCompleted(const bool IsCompleted)
	{
		this->IsCompleted = IsCompleted;
	}

private:
	std::string TodoListId;
	std::string Title;
	std::string Note;
	bool IsCompleted = false;
};

