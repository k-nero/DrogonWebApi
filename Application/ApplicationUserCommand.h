#pragma once
#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "ApplicationUser.h"
#include "IApplicationUserCommand.h"

class APPLICATION_API ApplicationUserCommand : public BaseCommand<ApplicationUser>, IApplicationUserCommand
{
public:
	ApplicationUserCommand();
	//explicit ApplicationUserCommand(SAConnection* con) : BaseCommand(con) {}
	~ApplicationUserCommand();
};

