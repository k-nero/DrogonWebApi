#include "TodoListRPC.h"

void TodoListRPC::InitCompletionQueue(ServerBuilder* builder)
{
	builder->RegisterService(&service_);

	for (int i = 0; i < 4; i++)
	{
		server_queue.push_back(builder->AddCompletionQueue());
	}
	
}

void TodoListRPC::Invoke()
{
	std::vector<std::thread> threads;

	threads.push_back(std::thread(&TodoListRPC::HandleRpcs<GetTodoListRPC>,this, server_queue[0].get()));

	for (auto& thread : threads)
	{
		thread.detach();
	}
}

