#pragma once
#include "BaseQuery.h"
#include "ApplicationApi.h"
#include "ApplicationUser.h"

class APPLICATION_API ApplicationUserQuery : public BaseQuery<ApplicationUser>
{
public:
	ApplicationUserQuery();
	//explicit ApplicationUserQuery(SAConnection* con) : BaseQuery(con) {}
	~ApplicationUserQuery();
};

