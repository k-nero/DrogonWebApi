#pragma once
#include <ApplicationUser.h>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "ApplicationUserCommand.h"
#include "ApplicationUserQuery.h"


class APPLICATION_API ApplicationUserService
{
public:
	ApplicationUserService();
	//explicit ApplicationUserService(SAConnection* con) { this->con = con; }
	std::shared_ptr<ApplicationUser> GetApplicationUserById(const std::string& id);
	std::shared_ptr<ApplicationUser> GetApplicationUserByUserName(std::string& userName);
	std::vector<std::shared_ptr<ApplicationUser>>GetAllApplicationUsers();
	int CreateApplicationUser(ApplicationUser* applicationUser);
	int UpdateApplicationUser(ApplicationUser* applicationUser);
	~ApplicationUserService();
private:
	//SAConnection* con;
};

