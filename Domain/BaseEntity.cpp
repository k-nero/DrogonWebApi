#include "pch.h"
#include "BaseEntity.h"

BaseEntity::BaseEntity()
{
}

BaseEntity::BaseEntity(const BaseEntity& base_entity)
{
	Id = base_entity.Id;
	CreatedDate = base_entity.CreatedDate;
	ModifiedDate = base_entity.ModifiedDate;
}

BaseEntity::BaseEntity(BaseEntity&& base_entity) noexcept
{
	Id = std::move(base_entity.Id);
	CreatedDate = std::move(base_entity.CreatedDate);
	ModifiedDate = std::move(base_entity.ModifiedDate);
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
