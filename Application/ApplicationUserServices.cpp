#include "pch.h"
#include "ApplicationUserServices.h"
#include "DbContext.h"

ApplicationUserService::ApplicationUserService()
{
	
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserById( const std::string& id ) noexcept(false)
{
	try
	{
		DbContext db;
		ApplicationUserQuery query(db.GetConnection());
		return query.GetById(id);
	}
	catch ( std::exception& ex )
	{
		throw ex;
	}
	return nullptr;
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserByUserName( std::string& userName ) noexcept(false)
{
	try
	{
		DbContext db;
		ApplicationUserQuery query(db.GetConnection());
		return query.GetSingle("UserName = '" + userName + "'");
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return nullptr;
}

std::vector<std::shared_ptr<ApplicationUser>> ApplicationUserService::GetAllApplicationUsers() noexcept(false)
{
	std::vector<std::shared_ptr<ApplicationUser>> applicationUsers;
	try
	{
		DbContext db;
		ApplicationUserQuery query(db.GetConnection());
		applicationUsers = query.GetAll();
		return applicationUsers;
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return applicationUsers;
}


int ApplicationUserService::CreateApplicationUser(ApplicationUser* applicationUser) noexcept(false)
{
	try
	{
		DbContext db;
		ApplicationUserCommand cmd(db.GetConnection());
		return cmd.Create(applicationUser);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return 0;
}

int ApplicationUserService::UpdateApplicationUser(ApplicationUser* applicationUser) noexcept(false)
{
	try
	{
		DbContext db;
		ApplicationUserCommand cmd(db.GetConnection());
		return cmd.Update(applicationUser);
	}
	catch (std::exception& ex)
	{
		throw ex;
	}
	return 0;
}

ApplicationUserService::~ApplicationUserService()
{
	
}

