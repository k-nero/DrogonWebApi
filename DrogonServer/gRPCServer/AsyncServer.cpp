#include "AsyncServer.h"

void AsyncServer::Run(uint16_t port)
{
	std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();

	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service_" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *asynchronous* service.
	builder.RegisterService(&service_);
	// Get hold of the completion queue used for the asynchronous communication
	// with the gRPC runtime.
	cq_ = builder.AddCompletionQueue();
	// Finally assemble the server.
	server_ = builder.BuildAndStart();
	std::cout << "Server listening on " << server_address << std::endl;

	// Proceed to the server's main loop.
	HandleRpcs();
}

void AsyncServer::HandleRpcs()
{
	// Spawn a new CallData instance to serve new clients.
	new GetTodoListRPC(&service_, cq_.get());
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
		static_cast<GetTodoListRPC*>(tag)->Proceed();
	}
}
