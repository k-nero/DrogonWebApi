#pragma once
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

template<class S>
class RPCInterface
{
public:
	virtual ~RPCInterface() = default;
	virtual void Proceed()
	{
		assert("Proceed method should be overridden");
	}
	RPCInterface(S::AsyncService* service, ServerCompletionQueue* cq) : service_(service), cq_(cq), status_(CREATE) 
	{
		
	}

protected:
	ServerCompletionQueue* cq_;
	S::AsyncService* service_;
	// The producer-consumer queue where for asynchronous server notifications.

	// Context for the rpc, allowing to tweak aspects of it such as the use
	// of compression, authentication, as well as to send metadata back to the
	// client.
	ServerContext ctx_;

	// Let's implement a tiny state machine with the following states.
	CallStatus status_;  // The current serving state.
};