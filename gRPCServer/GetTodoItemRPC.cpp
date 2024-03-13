#include "GetTodoItemRPC.h"
#include "TodoItemService.h"


void GetTodoItemRPC::Proceed()
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
		service_->RequestGetTodoItems(&ctx_, &request_, &responder_, cq_, cq_, this);
	}
	else if (status_ == PROCESS)
	{
		// Spawn a new CallData instance to serve new clients while we process
		// the one for this CallData. The instance will deallocate itself as
		// part of its FINISH state.
		new GetTodoItemRPC(service_, cq_);

		// The actual processing.
		std::string id = request_.id();

		if (id.empty())
		{
			reply_.set_message("Bad Request, id is empty");
			reply_.set_status(400);
			status_ = FINISH;
			responder_.Finish(reply_, Status::CANCELLED, this);
			return;
		}

		auto todo_item = TodoItemService().GetTodoItemById(id);
		if (todo_item == nullptr)
		{
			reply_.set_message("Not Found");
			reply_.set_status(404);
			status_ = FINISH;
			responder_.Finish(reply_, Status::CANCELLED, this);
			return;
		};

		todo_item::TodoItem* rs = new todo_item::TodoItem();

		rs->set_id(todo_item->GetId());
		rs->set_title(todo_item->GetTitle());
		rs->set_note(todo_item->GetNote());
		rs->set_completed(todo_item->GetIsCompleted());
		rs->set_created_date(todo_item->GetCreatedDate());
		rs->set_modified_date(todo_item->GetModifiedDate());
		rs->set_todo_list_id(todo_item->GetTodoListId());
		rs->set_version(todo_item->GetVersion());


		reply_.set_message("Success");
		reply_.set_status(200);

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