#pragma once
#include <string>
#include <boost/describe.hpp>
#include "DomainApi.h"

/// <summary>
///  It is the base class for all entities in the system. It contains the common properties of all entities.
///  It is a part of the underlying command and query model in Application module
/// </summary>
class DOMAIN_API BaseEntity
{
	BOOST_DESCRIBE_CLASS(BaseEntity, (), (), (Id, CreatedDate, ModifiedDate, Version), ())

public:
	BaseEntity();

	//Copy constructor
	BaseEntity(const BaseEntity& baseentity);

	//Move constructor
	BaseEntity(BaseEntity&& baseentity) noexcept;

	//Copy assignment operator
	BaseEntity& operator=(const BaseEntity& baseentity);

	BaseEntity(const std::string& id, const std::string& createdDate = "", const std::string& modifedDate = "");

	//Move assignment operator
	BaseEntity& operator=(BaseEntity&& baseentity) noexcept;

	virtual std::string ToString();

	virtual std::string GetId()
	{
		return Id;
	}
	virtual void SetId(const std::string& id)
	{
		Id = id;
	}
	virtual std::string GetCreatedDate()
	{
		return CreatedDate;
	}
	virtual void SetCreatedDate(const std::string& createdDate)
	{
		CreatedDate = createdDate;
	}
	virtual std::string GetModifiedDate()
	{
		return ModifiedDate;
	}
	virtual void SetModifiedDate(const std::string& modifiedDate)
	{
		ModifiedDate = modifiedDate;
	}

	virtual int GetVersion()
	{
		return Version;
	}

	virtual void SetVersion(int version)
	{
		Version = version;
	}
	virtual ~BaseEntity();
protected:
	/// <summary>
	///  It is the unique identifier of the entity. Its name should not be changed or deleted.
	/// </summary>
	std::string Id = "";
	std::string CreatedDate = "";
	std::string ModifiedDate = "";
	int Version = 0;
};

