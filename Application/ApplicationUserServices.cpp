#include "pch.h"
#include "ApplicationUserServices.h"
#include "DbContext.h"

ApplicationUserService::ApplicationUserService()
{

}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserById(const std::string& id) noexcept(false)
{
	ApplicationUserQuery query;
	return query.GetById(id);
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserByUserName(std::string& UserName) noexcept(false)
{
	ApplicationUserQuery query;
	return query.GetSingle(EQ(UserName));
}

std::vector<std::shared_ptr<ApplicationUser>> ApplicationUserService::GetAllApplicationUsers() noexcept(false)
{
	ApplicationUserQuery query;
	return query.GetAll();
}

std::string ApplicationUserService::CreateApplicationUser(ApplicationUser* applicationUser) noexcept(false)
{
	std::string id = CoreHelper::CreateUUID();
	applicationUser->SetId(id);
	ApplicationUserCommand cmd;
	cmd.Create(applicationUser);
	return id;
}

int ApplicationUserService::UpdateApplicationUser(ApplicationUser* applicationUser) noexcept(false)
{
	DbContext db;
	ApplicationUserCommand cmd;
	return cmd.Update(applicationUser);
}

ApplicationUserService::~ApplicationUserService()
{

}

