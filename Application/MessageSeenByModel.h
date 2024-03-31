#pragma once
#include <boost/describe.hpp>
#include "string"

class MessageSeenByModel
{
	BOOST_DESCRIBE_CLASS(MessageSeenByModel, (), (), (MessageId, ApplicationUserId), ())
public:
	MessageSeenByModel() = default;
	~MessageSeenByModel() = default;

	MessageSeenByModel(const std::string& messageId, const std::string& applicationUserId)
		: MessageId(messageId), ApplicationUserId(applicationUserId)
	{
	}

	MessageSeenByModel(const MessageSeenByModel& other)
		: MessageId(other.MessageId), ApplicationUserId(other.ApplicationUserId)
	{
	}

	MessageSeenByModel& operator=(const MessageSeenByModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		MessageId = other.MessageId;
		ApplicationUserId = other.ApplicationUserId;
		return *this;
	}

	MessageSeenByModel(MessageSeenByModel&& other) noexcept
		: MessageId(std::move(other.MessageId)), ApplicationUserId(std::move(other.ApplicationUserId))
	{
	}

	MessageSeenByModel& operator=(MessageSeenByModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		MessageId = std::move(other.MessageId);
		ApplicationUserId = std::move(other.ApplicationUserId);
		return *this;
	}



	std::string GetMessageId() const { return MessageId; }
	void SetMessageId(const std::string& val) { MessageId = val; }

	std::string GetApplicationUserId() const { return ApplicationUserId; }
	void SetApplicationUserId(const std::string& val) { ApplicationUserId = val; }


protected:
	std::string MessageId = "";
	std::string ApplicationUserId = "";
};

