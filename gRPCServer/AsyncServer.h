#pragma once


#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include "GetTodoListRPC.h"
#include "TodoItemRPC.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

class AsyncServer final
{

public:
	~AsyncServer()
	{
		server_->Shutdown();
		// Always shutdown the completion queue after the server.
	}

	// There is no shutdown handling in this code.
	void Run(uint16_t port);

private:
	TodoItemRPC todo_item_service;
	std::unique_ptr<Server> server_;
};

