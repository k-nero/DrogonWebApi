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
	
	todo_item_service.InitCompletionQueue(&builder);
	todo_list_service.InitCompletionQueue(&builder);

	// Get hold of the completion queue used for the asynchronous communication
	// with the gRPC runtime.
	// Finally assemble the server.
	server_ = builder.BuildAndStart();

	todo_item_service.Invoke();
	todo_list_service.Invoke();

	server_->Wait();

	std::cout << "Server listening on " << server_address << std::endl;

	// Proceed to the server's main loop.
}

