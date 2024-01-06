#pragma once
#include "ApplicationUser.h"
#include "ApplicationApi.h"
#include "BaseQuery.h"

class APPLICATION_API TestApplicationUser : public BaseQuery<ApplicationUser>
{
public:
	TestApplicationUser() = default;
	TestApplicationUser(SAConnection* con) : BaseQuery<ApplicationUser>(con) {}
	~TestApplicationUser() = default;
};

