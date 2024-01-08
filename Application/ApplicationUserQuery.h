#pragma once
#include "BaseQuery.h"
#include "IAppicationUserQuery.h"
#include "ApplicationApi.h"

class APPLICATION_API ApplicationUserQuery : public BaseQuery<ApplicationUser>, IApplicationUserQuery
{
public:
	ApplicationUserQuery();
	explicit ApplicationUserQuery(SAConnection* con) : BaseQuery(con) {}
	~ApplicationUserQuery();
};

