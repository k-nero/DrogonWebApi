#include "pch.h"
#include "ApplicationUserServices.h"
#include "DbContext.h"

ApplicationUserService::ApplicationUserService()
{

}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserById(const std::string& id) noexcept(false)
{
	DbContext db;
	ApplicationUserQuery query(db.GetConnection());
	return query.GetById(id);
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserByUserName(std::string& userName) noexcept(false)
{
	DbContext db;
	ApplicationUserQuery query(db.GetConnection());
	return query.GetSingle("UserName = '" + userName + "'");
}

std::vector<std::shared_ptr<ApplicationUser>> ApplicationUserService::GetAllApplicationUsers() noexcept(false)
{
	std::vector<std::shared_ptr<ApplicationUser>> applicationUsers;
	DbContext db;
	ApplicationUserQuery query(db.GetConnection());
	applicationUsers = query.GetAll();
	return applicationUsers;
}


int ApplicationUserService::CreateApplicationUser(ApplicationUser* applicationUser) noexcept(false)
{
	DbContext db;
	ApplicationUserCommand cmd(db.GetConnection());
	return cmd.Create(applicationUser);
}

int ApplicationUserService::UpdateApplicationUser(ApplicationUser* applicationUser) noexcept(false)
{
	DbContext db;
	ApplicationUserCommand cmd(db.GetConnection());
	return cmd.Update(applicationUser);
}

ApplicationUserService::~ApplicationUserService()
{

}

