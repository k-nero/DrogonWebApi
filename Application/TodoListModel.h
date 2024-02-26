#pragma once
#include "ApplicationApi.h"
#include <boost/describe.hpp>
#include <string>

class APPLICATION_API TodoListModel
{
	BOOST_DESCRIBE_CLASS(TodoListModel, (), (), (), (Title, Description))
public:
	TodoListModel() = default;
	TodoListModel(std::string title, std::string description);
	TodoListModel(const TodoListModel& other);
	TodoListModel& operator=(const TodoListModel& other);
	TodoListModel(TodoListModel&& other) noexcept;
	TodoListModel& operator=(TodoListModel&& other) noexcept;
	~TodoListModel() = default;

	std::string GetTitle() const
	{
		return Title;
	}
	void SetTitle(const std::string& title)
	{
		Title = title;
	}
	std::string GetDescription() const
	{
		return Description;
	}
	void SetDescription(const std::string& description)
	{
		Description = description;
	}

private:
	std::string Title = {};
	std::string Description = {};
};

