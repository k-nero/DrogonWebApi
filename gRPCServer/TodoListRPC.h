#pragma once
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
class TodoListRPC
{
public:
	TodoListRPC() = default;
	~TodoListRPC()
	{
		for (auto& queue : server_queue)
		{
			queue->Shutdown();
		}

	}

	void InitCompletionQueue(ServerBuilder* builder);

	void Invoke();
private:

	std::vector<std::unique_ptr<ServerCompletionQueue>> server_queue;
	todo_list::TodoListService::AsyncService service_;
	void HandleRpcs(ServerCompletionQueue* cq_);

};

