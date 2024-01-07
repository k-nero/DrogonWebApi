#pragma once
#include "BaseQuery.h"
#include "IAppicationUserQuery.h"

class ApplicationUserQuery : public BaseQuery<ApplicationUser>, IApplicationUserQuery
{
public:
	ApplicationUserQuery();
	explicit ApplicationUserQuery(SAConnection* con) : BaseQuery(con) {}
	virtual std::shared_ptr<ApplicationUser> GetByUserName();
	~ApplicationUserQuery();
};

