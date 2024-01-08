#include "pch.h"
#include "ApplicationUserServices.h"
#include <iostream>
#include <ostream>
#include <SQLAPI.h>
#include "DbContext.h"

ApplicationUserService::ApplicationUserService()
{
	
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserById( const std::string& id ) 
{
	try
	{
		DbContext db;
		ApplicationUserQuery query(db.GetConnection());
		return query.GetById(id);
	}
	catch ( SAException& ex )
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return nullptr;
}

std::shared_ptr<ApplicationUser> ApplicationUserService::GetApplicationUserByUserName( std::string& userName )
{
	try
	{
		DbContext db;
		ApplicationUserQuery query(db.GetConnection());
		return query.GetSingle("UserName = '" + userName + "'");
	}
	catch ( SAException& ex )
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return nullptr;
}

std::vector<std::shared_ptr<ApplicationUser>> ApplicationUserService::GetAllApplicationUsers()
{
	std::vector<std::shared_ptr<ApplicationUser>> applicationUsers;
	try
	{
		DbContext db;
		ApplicationUserQuery query(db.GetConnection());
		applicationUsers = query.GetAll();
		return applicationUsers;
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return applicationUsers;
}


int ApplicationUserService::CreateApplicationUser(ApplicationUser* applicationUser)
{
	try
	{
		DbContext db;
		ApplicationUserCommand cmd(db.GetConnection());
		return cmd.Create(applicationUser);
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database command failed");
	}
	return 0;
}

int ApplicationUserService::UpdateApplicationUser(ApplicationUser* applicationUser)
{
	try
	{
		DbContext db;
		ApplicationUserCommand cmd(db.GetConnection());
		return cmd.Update(applicationUser);
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database command failed");
	}
	return 0;
}

ApplicationUserService::~ApplicationUserService()
{
	
}

