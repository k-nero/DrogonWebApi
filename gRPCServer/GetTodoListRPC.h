#pragma once

#ifdef BAZEL_BUILD
#include "proto/todo_list.grpc.pb.h"
#else
#include "proto/todo_list.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

class GetTodoListRPC
{
public:

	GetTodoListRPC(todo_list::TodoListService::AsyncService* service, ServerCompletionQueue* cq) : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
	{
		// Invoke the serving logic right away.
		Proceed();
	}

	void Proceed()
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
			std::string prefix("Hello ");
			reply_.set_message(prefix + request_.id());

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

protected:
	ServerCompletionQueue* cq_;
	todo_list::TodoListService::AsyncService* service_;
	// The producer-consumer queue where for asynchronous server notifications.

	// Context for the rpc, allowing to tweak aspects of it such as the use
	// of compression, authentication, as well as to send metadata back to the
	// client.
	ServerContext ctx_;

	// What we get from the client.
	todo_list::GetTodoListRequest request_;
	// What we send back to the client.
	todo_list::GetTodoListResponse reply_;

	// The means to get back to the client.
	ServerAsyncResponseWriter< todo_list::GetTodoListResponse> responder_;

	// Let's implement a tiny state machine with the following states.
	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;  // The current serving state.
};

