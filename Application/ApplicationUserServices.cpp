#include "pch.h"
#include "ApplicationUserServices.h"

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserById(const std::string& id) noexcept(false)
{
	Query<ApplicationUser> query;
	return query.GetByIdEw(id);
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserByUserName(std::string& UserName) noexcept(false)
{
	Query<ApplicationUser> query;
	return query.GetSingleEw(EQ(UserName));
}

std::vector<std::shared_ptr<ApplicationUser>> ApplicationUserService::GetAllApplicationUsers() noexcept(false)
{
	Query<ApplicationUser> query;
	return query.GetAllEw();
}

std::string ApplicationUserService::CreateApplicationUser(ApplicationUser& applicationUser) noexcept(false)
{
	std::string id = CoreHelper::CreateUUID();
	applicationUser.SetId(id);
	BaseCommand<ApplicationUser> cmd;
	cmd.Create(applicationUser);
	return id;
}

int ApplicationUserService::UpdateApplicationUser(ApplicationUser& applicationUser, const std::string& Id) noexcept(false)
{
	BaseCommand<ApplicationUser> cmd;
	return cmd.Update(applicationUser, EQ(Id));
}

int ApplicationUserService::DeleteApplicationUser(const std::string& Id)
{
	BaseCommand<ApplicationUser> cmd;
	return cmd.Delete(EQ(Id));
}

