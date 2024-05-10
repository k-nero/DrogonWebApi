#pragma once
#include <grpcpp/grpcpp.h>
#include "RPCInterface.h"

template<class S>
class BaseService
{
public:
	virtual void InitCompletionQueue(grpc::ServerBuilder* builder) = 0;
	virtual void Invoke() = 0;
	virtual ~BaseService() = default;
protected:
	std::vector<std::unique_ptr<ServerCompletionQueue>> server_queue;
	S::AsyncService service_;
	template <class T, typename = std::is_base_of<RPCInterface<S>, T>::type>
	void HandleRpcs(ServerCompletionQueue* cq_)
	{
		new T(&service_, cq_);
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
			static_cast<T*>(tag)->Proceed();
		}
	}
};