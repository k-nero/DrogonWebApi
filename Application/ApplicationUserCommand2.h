#pragma once
#include <ApplicationUser.h>
#include <SQLAPI.h>
#include <vector>
#include "CoreHelper.h"
#include "ApplicationApi.h"


class APPLICATION_API ApplicationUserCommand2
{
	public:
		ApplicationUserCommand2();
		explicit ApplicationUserCommand2(SAConnection* con) { this->con = con; }
		std::shared_ptr<ApplicationUser> GetApplicationUserById(const std::string& id);
		std::shared_ptr<ApplicationUser> GetApplicationUserByUserName(std::string userName);
		std::vector<std::shared_ptr<ApplicationUser>>GetAllApplicationUsers();
		int CreateApplicationUser(ApplicationUser* applicationUser);
		int UpdateApplicationUser(ApplicationUser* applicationUser);
		~ApplicationUserCommand2();
	private:
		std::shared_ptr<ApplicationUser> GetApplicationUserFromCommand(SACommand& cmd);
		SAConnection* con = nullptr;
};

