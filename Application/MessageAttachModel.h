#pragma once
#include <boost/describe.hpp>
#include "string"
class MessageAttachModel
{
	BOOST_DESCRIBE_CLASS(MessageAttachModel, (), (), (AttachName, AttachUrl, MessageId, AttachType), ())
public:
	MessageAttachModel() = default;
	~MessageAttachModel() = default;

	MessageAttachModel(const std::string& attachName, const std::string& attactUrl, const std::string& messageId, const std::string& attachType)
		: AttachName(attachName), AttachUrl(attactUrl), MessageId(messageId), AttachType(attachType)
	{
	}

	MessageAttachModel(const MessageAttachModel& other)
		: AttachName(other.AttachName), AttachUrl(other.AttachUrl), MessageId(other.MessageId), AttachType(other.AttachType)
	{
	}

	MessageAttachModel& operator=(const MessageAttachModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		AttachName = other.AttachName;
		AttachUrl = other.AttachUrl;
		MessageId = other.MessageId;
		AttachType = other.AttachType;
		return *this;
	}

	MessageAttachModel(MessageAttachModel&& other) noexcept
		: AttachName(std::move(other.AttachName)), AttachUrl(std::move(other.AttachUrl)), MessageId(std::move(other.MessageId)), AttachType(std::move(other.AttachType))
	{
	}

	MessageAttachModel& operator=(MessageAttachModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		AttachName = std::move(other.AttachName);
		AttachUrl = std::move(other.AttachUrl);
		MessageId = std::move(other.MessageId);
		AttachType = std::move(other.AttachType);
		return *this;
	}

	std::string GetAttachName() const { return AttachName; }
	std::string GetAttactUrl() const { return AttachUrl; }
	std::string GetMessageId() const { return MessageId; }
	std::string GetAttachType() const { return AttachType; }

	void SetAttachName(const std::string& attachName) { AttachName = attachName; }
	void SetAttactUrl(const std::string& attactUrl) { AttachUrl = attactUrl; }
	void SetMessageId(const std::string& messageId) { MessageId = messageId; }
	void SetAttachType(const std::string& attachType) { AttachType = attachType; }
protected:
	std::string AttachName = "";
	std::string AttachUrl = "";
	std::string MessageId = "";
	std::string AttachType = "";
};

