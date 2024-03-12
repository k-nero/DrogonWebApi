#pragma once

#include "Enum.h"

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
		Proceed();
	}

	void Proceed();

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
	CallStatus status_;  // The current serving state.
};

