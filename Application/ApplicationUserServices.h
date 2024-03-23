#pragma once
#include <ApplicationUser.h>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include "BaseQuery.h"
#include "BaseCommand.h"


class APPLICATION_API ApplicationUserService
{
public:
	ApplicationUserService() = default;
	std::shared_ptr<ApplicationUser> GetApplicationUserById(const std::string& id);
	std::shared_ptr<ApplicationUser> GetApplicationUserByUserName(std::string& userName);
	std::vector<std::shared_ptr<ApplicationUser>>GetAllApplicationUsers();
	std::string CreateApplicationUser(ApplicationUser& applicationUser);
	int UpdateApplicationUser(ApplicationUser& applicationUser, const std::string& id);
	int DeleteApplicationUser(const std::string& id);
	~ApplicationUserService() = default;
};

