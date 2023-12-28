#include "pch.h"
#include "TodoListCommand.h"
#include "iostream"

TodoListCommand::TodoListCommand()
{
}

std::shared_ptr<TodoList> TodoListCommand::GetTodoListById(const std::string& id)
{
	try
	{
		SACommand cmd(con, _TSA("SELECT * FROM [dbo].[TodoLists] WHERE Id=:id"));
		const SAString idStr(id.c_str());
		cmd.Param(_TSA("id")).setAsString() = idStr;
		cmd.Execute();
		if (cmd.FetchNext())
		{
			return GetTodoListFromCommand(cmd);
		}
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return nullptr;
}

std::vector<std::shared_ptr<TodoList>> TodoListCommand::GetAllTodoLists()
{
	std::vector<std::shared_ptr<TodoList>> todo_list;
	try
	{
		SACommand cmd(con, _TSA("SELECT * FROM [dbo].[TodoLists]"));
		cmd.Execute();
		while (cmd.FetchNext())
		{
			auto tl = GetTodoListFromCommand(cmd);
			tl->TodoItems = std::make_shared<TodoItemCommand>(con)->GetAllTodoItems("TodoListId = '" + tl->GetId() + "'");
			todo_list.push_back(tl);
		}
		return todo_list;
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return todo_list;
}

int TodoListCommand::CreateTodoList(TodoList* todoList)
{
	return 0;
}

int TodoListCommand::UpdateTodoList(TodoList* todoList)
{
	return 0;
}

TodoListCommand::~TodoListCommand()
{
	if (this->con != nullptr)
	{
		con = nullptr;
	}
}

std::shared_ptr<TodoList> TodoListCommand::GetTodoListFromCommand(SACommand& cmd)
{
	return std::shared_ptr<TodoList>(new TodoList(
			cmd.Field("Id").asString().GetMultiByteChars(),
			cmd.Field("Title").asString().GetMultiByteChars(),
			cmd.Field("Description").asString().GetMultiByteChars(),
			cmd.Field("CreatedDate").asString().GetMultiByteChars(),
			cmd.Field("ModifiedDate").asString().GetMultiByteChars()));
}