#pragma once
#include <boost/describe.hpp>
#include <string>

class ContactListModel
{
	BOOST_DESCRIBE_CLASS(ContactListModel, (), (), (ApplicationUserId, Name), ())
public:
	ContactListModel() = default;
	~ContactListModel() = default;

	ContactListModel(const std::string& applicationUserId, const std::string& name) : ApplicationUserId(applicationUserId), Name(name) {}

	ContactListModel(const ContactListModel& other) : ApplicationUserId(other.ApplicationUserId), Name(other.Name) {}
	ContactListModel& operator=(const ContactListModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		ApplicationUserId = other.ApplicationUserId;
		Name = other.Name;
		return *this;
	}

	ContactListModel(ContactListModel&& other) noexcept : ApplicationUserId(std::move(other.ApplicationUserId)), Name(std::move(other.Name)) {}

	ContactListModel& operator=(ContactListModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		ApplicationUserId = std::move(other.ApplicationUserId);
		Name = std::move(other.Name);
		return *this;
	}

	std::string GetApplicationUserId() const { return ApplicationUserId; }
	void SetApplicationUserId(const std::string& val) { ApplicationUserId = val; }

	std::string GetName() const { return Name; }
	void SetName(const std::string& val) { Name = val; }

protected:
	std::string ApplicationUserId = "";
	std::string Name = "";
};

