#include "GetTodoListRPC.h"
#include "TodoListService.h"


void GetTodoListRPC::Proceed()
{
	if (status_ == CREATE)
	{
		// Make this instance progress to the PROCESS state.
		status_ = PROCESS;

		// As part of the initial CREATE state, we *request* that the system
		// start processing SayHello requests. In this request, "this" acts are
		// the tag uniquely identifying the request (so that different CallData
		// instances can serve different requests concurrently), in this case
		// the memory address of this CallData instance.
		service_->RequestGetTodoList(&ctx_, &request_, &responder_, cq_, cq_, this);
	}
	else if (status_ == PROCESS)
	{
		// Spawn a new CallData instance to serve new clients while we process
		// the one for this CallData. The instance will deallocate itself as
		// part of its FINISH state.
		new GetTodoListRPC(service_, cq_);

		// The actual processing.
		std::string id = request_.id();

		if(id.empty())
		{
			reply_.set_message("Bad Request, id is empty");
			reply_.set_status(400);
			status_ = FINISH;
			responder_.Finish(reply_, Status::CANCELLED, this);
			return;
		}

		auto todo_list = TodoListService().GetTodoListById(id);
		if (todo_list == nullptr)
		{
			reply_.set_message("Not Found");
			reply_.set_status(404);
			status_ = FINISH;
			responder_.Finish(reply_, Status::CANCELLED, this);
			return;
		};

		todo_list::TodoList*rs = new todo_list::TodoList();

		rs->set_id(todo_list->GetId());
		rs->set_title(todo_list->GetTitle());
		rs->set_description(todo_list->GetDescription());
		rs->set_created_date(todo_list->GetCreatedDate());
		rs->set_modified_date(todo_list->GetModifiedDate());

		for (auto& todo_item : todo_list->GetTodoItems())
		{
			todo_list::TodoItem* item = new todo_list::TodoItem();
			item->set_id(todo_item->GetId());
			item->set_title(todo_item->GetTitle());
			item->set_note(todo_item->GetNote());
			item->set_completed(todo_item->GetIsCompleted());
			item->set_created_date(todo_item->GetCreatedDate());
			item->set_modified_date(todo_item->GetModifiedDate());
			rs->mutable_todo_item()->AddAllocated(item);
		}

		reply_.set_message("Success");
		reply_.set_status(200);
		reply_.set_allocated_todo_list(rs);

		status_ = FINISH;
		responder_.Finish(reply_, Status::OK, this);
	}
	else
	{
		GPR_ASSERT(status_ == FINISH);
		// Once in the FINISH state, deallocate ourselves (CallData).
		delete this;
	}
}