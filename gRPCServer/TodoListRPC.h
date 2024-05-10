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
#include "BaseService.h"

class TodoListRPC : public BaseService<todo_list::TodoListService>
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

	void InitCompletionQueue(ServerBuilder* builder) override;

	void Invoke() override;
};

