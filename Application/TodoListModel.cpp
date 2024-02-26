#include "pch.h"
#include "TodoListModel.h"

TodoListModel::TodoListModel(std::string title, std::string description)
{
	this->Title = title;
	this->Description = description;
}

TodoListModel::TodoListModel(const TodoListModel& other)
{
	this->Title = other.Title;
	this->Description = other.Description;
}

TodoListModel& TodoListModel::operator=(const TodoListModel& other) = default;

TodoListModel::TodoListModel(TodoListModel&& other) noexcept
{
	this->Title = std::move(other.Title);
	this->Description = std::move(other.Description);
}

TodoListModel& TodoListModel::operator=(TodoListModel&& other) noexcept = default;
