#include "pch.h"
#include "BaseEntity.h"

BaseEntity::BaseEntity()
{
}

BaseEntity::BaseEntity(const BaseEntity& baseentity)
{
	Id = baseentity.Id;
	CreatedDate = baseentity.CreatedDate;
	ModifiedDate = baseentity.ModifiedDate;
}

BaseEntity::BaseEntity(BaseEntity&& baseentity) noexcept
{
	Id = std::move(baseentity.Id);
	CreatedDate = std::move(baseentity.CreatedDate);
	ModifiedDate = std::move(baseentity.ModifiedDate);
}

BaseEntity& BaseEntity::operator=(const BaseEntity& baseentity)
= default;

BaseEntity::BaseEntity(const std::string& id, const std::string& createdDate, const std::string& modifedDate)
{
	Id = id;
	CreatedDate = createdDate;
	ModifiedDate = modifedDate;
}

BaseEntity& BaseEntity::operator=(BaseEntity&& baseentity) noexcept
= default;

std::string BaseEntity::ToString()
{
	return std::string();
}

BaseEntity::~BaseEntity()
{
}
