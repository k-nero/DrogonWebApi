#pragma once
#include <ApplicationUser.h>
#include <SQLAPI.h>
#include <vector>

class __declspec(dllexport) ApplicationUserCommand
{
	public:
		ApplicationUserCommand();
		ApplicationUserCommand(SAConnection* con) { this->con = con; }
		ApplicationUser* GetApplicationUserById(const std::string& id) const;
		ApplicationUser* GetApplicationUserByUserName(std::string userName);
		std::vector<ApplicationUser*> GetAllApplicationUsers();
		
		~ApplicationUserCommand();
	private:
		SAConnection* con = nullptr;
};

