#include "pch.h"
#include "TodoItemCommand.h"
#include "iostream"

TodoItemCommand::TodoItemCommand()
{
}

std::shared_ptr<TodoItem> TodoItemCommand::GetTodoItemById(const std::string& id)
{
	try
	{
		SACommand cmd(con, _TSA("SELECT * FROM [dbo].[TodoItems] WHERE Id=:id"));
		const SAString idStr(id.c_str());
		cmd.Param(_TSA("id")).setAsString() = idStr;
		cmd.Execute();
		if (cmd.FetchNext())
		{
			return GetTodoItemFromCommand(cmd);
		}
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return nullptr;
}

std::vector<std::shared_ptr<TodoItem>> TodoItemCommand::GetAllTodoItems(std::string query)
{
	std::vector<std::shared_ptr<TodoItem>> todo_items;
	try
	{
		std::string base_query = "SELECT * FROM [dbo].[TodoItems]";
		if (query.length() > 1)
		{
			base_query += " WHERE " + query;
		}
		SACommand cmd(con, _TSA(base_query.c_str()));
		cmd.Execute();
		while (cmd.FetchNext())
		{
			todo_items.push_back(GetTodoItemFromCommand(cmd));
		}
		return todo_items;
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return todo_items;
}

int TodoItemCommand::CreateTodoItem(TodoItem* todoItem)
{
	try 
{
		SACommand cmd(con, _TSA("INSERT INTO [dbo].[TodoItems] (Id, TodoListId, Title, Note, IsCompleted, CreatedDate) VALUES (:id, :todoListId, :title, :note, :isCompleted, :createdDate)"));
		cmd.Param(_TSA("id")).setAsString() = todoItem->GetId().c_str();
		cmd.Param(_TSA("todoListId")).setAsString() = todoItem->GetTodoListId().c_str();
		cmd.Param(_TSA("title")).setAsString() = todoItem->GetTitle().c_str();
		cmd.Param(_TSA("note")).setAsString() = todoItem->GetNote().c_str();
		cmd.Param(_TSA("isCompleted")).setAsBool() = todoItem->GetIsCompleted();
		cmd.Param(_TSA("createdDate")).setAsDateTime() = SADateTime(CoreHelper::GetSystemTime());
		cmd.Execute();
		return cmd.RowsAffected();
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database insert failed");
	}
	return 0;
}

int TodoItemCommand::UpdateTodoItem(TodoItem* todoItem)
{
	try
{
		SACommand cmd(con, _TSA("UPDATE [dbo].[TodoItems] SET TodoListId=:todoListId, Title=:title, Note=:note, IsCompleted=:isCompleted, ModifiedDate=:modifiedDate WHERE Id=:id"));
		cmd.Param(_TSA("id")).setAsString() = todoItem->GetId().c_str();
		cmd.Param(_TSA("todoListId")).setAsString() = todoItem->GetTodoListId().c_str();
		cmd.Param(_TSA("title")).setAsString() = todoItem->GetTitle().c_str();
		cmd.Param(_TSA("note")).setAsString() = todoItem->GetNote().c_str();
		cmd.Param(_TSA("isCompleted")).setAsBool() = todoItem->GetIsCompleted();
		cmd.Param(_TSA("modifiedDate")).setAsDateTime() = SADateTime(CoreHelper::GetSystemTime());
		cmd.Execute();
		return cmd.RowsAffected();
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database set failed");
	}
	return 0;
}

TodoItemCommand::~TodoItemCommand()
{
}

std::shared_ptr<TodoItem> TodoItemCommand::GetTodoItemFromCommand(SACommand& cmd)
{
	return std::shared_ptr<TodoItem>( new TodoItem(
		cmd.Field("Id").asString().GetMultiByteChars(),
		cmd.Field("TodoListId").asString().GetMultiByteChars(),
		cmd.Field("Title").asString().GetMultiByteChars(),
		cmd.Field("Note").asString().GetMultiByteChars(),
		cmd.Field("IsCompleted").asBool(),
		cmd.Field("CreatedDate").asString().GetMultiByteChars(),
		cmd.Field("ModifiedDate").asString().GetMultiByteChars()));
}
