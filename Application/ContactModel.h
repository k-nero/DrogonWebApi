#pragma once

#include <boost/describe.hpp>
#include <string>

class ContactModel
{
	BOOST_DESCRIBE_CLASS(ContactModel, (), (), (ApplicationUserId, ContactListId), ())
public:
	ContactModel() = default;
	~ContactModel() = default;

	ContactModel(const std::string& applicationUserId, const std::string& contactListId, const std::string& tag)
		: ApplicationUserId(applicationUserId), ContactListId(contactListId), Tag(tag)
	{
	}

	ContactModel(const ContactModel& other)
		: ApplicationUserId(other.ApplicationUserId), ContactListId(other.ContactListId), Tag(other.Tag)
	{
	}

	ContactModel& operator=(const ContactModel& other)
	{
		if(this == &other)
		{
			return *this;
		}

		ApplicationUserId = other.ApplicationUserId;
		ContactListId = other.ContactListId;
		Tag = other.Tag;
		return *this;
	}

	ContactModel(ContactModel&& other) noexcept
		: ApplicationUserId(std::move(other.ApplicationUserId)), ContactListId(std::move(other.ContactListId)), Tag(std::move(other.Tag))
	{
	}

	ContactModel& operator=(ContactModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		ApplicationUserId = std::move(other.ApplicationUserId);
		ContactListId = std::move(other.ContactListId);
		Tag = std::move(other.Tag);
		return *this;
	}



	std::string GetApplicationUserId() const { return ApplicationUserId; }
	void SetApplicationUserId(const std::string& value) { ApplicationUserId = value; }

	std::string GetContactListId() const { return ContactListId; }
	void SetContactListId(const std::string& value) { ContactListId = value; }

	std::string GetTag() const { return Tag; }
	void SetTag(const std::string& value) { Tag = value; }



protected:
	std::string ApplicationUserId = "";
	std::string ContactListId = "";
	std::string Tag = "";
};

