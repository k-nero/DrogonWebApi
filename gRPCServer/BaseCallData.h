#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

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

class BaseCallData
{
public:
	// Take in the "service" instance (in this case representing an asynchronous
	// server) and the completion queue "cq" used for asynchronous communication
	// with the gRPC runtime.
	BaseCallData(todo_list::TodoListService::AsyncService* service, ServerCompletionQueue* cq) : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
	{
		// Invoke the serving logic right away.
		Proceed();
	}

	void Proceed();

private:
	// The means of communication with the gRPC runtime for an asynchronous
	// server.
	todo_list::TodoListService::AsyncService* service_;
	// The producer-consumer queue where for asynchronous server notifications.
	ServerCompletionQueue* cq_;
	// Context for the rpc, allowing to tweak aspects of it such as the use
	// of compression, authentication, as well as to send metadata back to the
	// client.
	ServerContext ctx_;

	// What we get from the client.
	todo_list::GetTodoListRequest request_;
	// What we send back to the client.
	todo_list::GetTodoListResponse reply_;

	// The means to get back to the client.
	ServerAsyncResponseWriter<todo_list::GetTodoListResponse> responder_;

	// Let's implement a tiny state machine with the following states.
	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;  // The current serving state.
};

