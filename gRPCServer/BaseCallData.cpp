#include "BaseCallData.h"

void BaseCallData::Proceed()
{
	if (status_ == CREATE)
	{
		// Make this instance progress to the PROCESS state.
		status_ = PROCESS;

		// As part of the initial CREATE state, we *request* that the system
		// start processing SayHello requests. In this request, "this" acts are
		// the tag uniquely identifying the request (so that different CallData
		// instances can serve different requests concurrently), in this case
		// the memory address of this CallData instance.
		service_->RequestGetTodoList(&ctx_, &request_, &responder_, cq_, cq_,
			this);
	}
	else if (status_ == PROCESS)
	{
		// Spawn a new CallData instance to serve new clients while we process
		// the one for this CallData. The instance will deallocate itself as
		// part of its FINISH state.
		new BaseCallData(service_, cq_);

		// The actual processing.
		std::string prefix("Hello ");
		reply_.set_message(prefix + request_.id());

		// And we are done! Let the gRPC runtime know we've finished, using the
		// memory address of this instance as the uniquely identifying tag for
		// the event.
		status_ = FINISH;
		responder_.Finish(reply_, Status::OK, this);
	}
	else
	{
		GPR_ASSERT(status_ == FINISH);
		// Once in the FINISH state, deallocate ourselves (CallData).
		delete this;
	}
}
