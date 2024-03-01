#pragma once
#include "ApplicationApi.h"
#include "BaseCommand.h"
#include "ApplicationUser.h"

class APPLICATION_API ApplicationUserCommand : public BaseCommand<ApplicationUser>
{
public:
	ApplicationUserCommand();
	//explicit ApplicationUserCommand(SAConnection* con) : BaseCommand(con) {}
	~ApplicationUserCommand();
};