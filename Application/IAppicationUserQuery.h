#pragma once
#include "ApplicationApi.h"
#include "ApplicationUser.h"
#include "IBaseQuery.h"

class APPLICATION_API IApplicationUserQuery : IBaseQuery<ApplicationUser>
{
	virtual std::shared_ptr<ApplicationUser> GetByUserName() = 0;
};