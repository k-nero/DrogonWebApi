#pragma once

#include "Enum.h"
#include "RPCInterface.h"

#ifdef BAZEL_BUILD
#include "proto/todo_item.grpc.pb.h"
#else
#include "proto/todo_item.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

class GetTodoItemRPC : RPCInterface<todo_item::TodoItemService>
{
public:

	GetTodoItemRPC(todo_item::TodoItemService::AsyncService* service, ServerCompletionQueue* cq) : RPCInterface(service, cq), responder_(&ctx_)
	{
		Proceed();
	}

	void Proceed() override;

protected:
	// What we get from the client.
	todo_item::GetTodoItemsRequest request_;
	// What we send back to the client.
	todo_item::GetTodoItemsResponse reply_;
	// The means to get back to the client.
	ServerAsyncResponseWriter< todo_item::GetTodoItemsResponse> responder_;
};

