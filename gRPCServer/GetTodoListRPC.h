#pragma once

#include "Enum.h"
#include "RPCInterface.h"
#ifdef BAZEL_BUILD
#include "proto/todo_list.grpc.pb.h"
#else
#include "proto/todo_list.grpc.pb.h"
#endif



class GetTodoListRPC : RPCInterface<todo_list::TodoListService>
{
public:

	GetTodoListRPC(todo_list::TodoListService::AsyncService* service, ServerCompletionQueue* cq) : RPCInterface(service, cq), responder_(&ctx_) 
	{
		Proceed();
	};

	void Proceed() override;

protected:

	// What we get from the client.
	todo_list::GetTodoListRequest request_;
	// What we send back to the client.
	todo_list::GetTodoListResponse reply_;

	// The means to get back to the client.
	ServerAsyncResponseWriter< todo_list::GetTodoListResponse> responder_;
};

