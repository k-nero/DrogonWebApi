#pragma once
#include <ApplicationUser.h>
#include <SQLAPI.h>
#include <vector>

class __declspec(dllexport) ApplicationUserCommand
{
	public:
		ApplicationUserCommand();
		explicit ApplicationUserCommand(SAConnection* con) { this->con = con; }
		ApplicationUser* GetApplicationUserById(const std::string& id) const;
		ApplicationUser* GetApplicationUserByUserName(std::string userName);
		std::vector<ApplicationUser*> GetAllApplicationUsers();
		std::string CreateApplicationUser(ApplicationUser* applicationUser);
		~ApplicationUserCommand();
	private:
		SAConnection* con = nullptr;
};

