#include "TodoItemRPC.h"

void TodoItemRPC::InitCompletionQueue(ServerBuilder* builder)
{
	builder->RegisterService(&service_);
	
	for (int i = 0; i < 4; i++)
	{
		server_queue.push_back(builder->AddCompletionQueue());
	}

	for (int i = 0; i < 4; i++)
	{
		std::thread(&TodoItemRPC::HandleRpcs, this, server_queue[i].get()).detach();
	}
}

void TodoItemRPC::HandleRpcs(ServerCompletionQueue* cq_)
{
	new GetTodoItemRPC(&service_, cq_);
	void* tag;  // uniquely identifies a request.
	bool ok;
	while (true)
	{
		// Block waiting to read the next event from the completion queue. The
		// event is uniquely identified by its tag, which in this case is the
		// memory address of a CallData instance.
		// The return value of Next should always be checked. This return value
		// tells us whether there is any kind of event or cq_ is shutting down.
		GPR_ASSERT(cq_->Next(&tag, &ok));
		GPR_ASSERT(ok);
		static_cast<GetTodoItemRPC*>(tag)->Proceed();
	}
}
