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
#include "GetTodoItemRPC.h"
#include "BaseService.h"
#include <boost/log/trivial.hpp>

class TodoItemRPC : public BaseService<todo_item::TodoItemService>
{
public:
	TodoItemRPC() = default;
	~TodoItemRPC()
	{
		for (auto& queue : server_queue)
		{
			queue->Shutdown();
		}
	}

	void InitCompletionQueue(ServerBuilder* builder) override;
	void Invoke() override;
};

