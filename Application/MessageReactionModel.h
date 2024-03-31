#pragma once

#include <boost/describe.hpp>
#include "string"

class MessageReactionModel
{
	BOOST_DESCRIBE_CLASS(MessageReactionModel, (), (), (MessageId, ApplicationUserId, ReactionType), ())
public:
	MessageReactionModel() = default;
	~MessageReactionModel() = default;

	MessageReactionModel(const std::string& messageId, const std::string& applicationUserId, const std::string& reactionType)
		: MessageId(messageId), ApplicationUserId(applicationUserId), ReactionType(reactionType)
	{
	}

	MessageReactionModel(const MessageReactionModel& other)
		: MessageId(other.MessageId), ApplicationUserId(other.ApplicationUserId), ReactionType(other.ReactionType)
	{
	}

	MessageReactionModel& operator=(const MessageReactionModel& other)
	{
		if (this == &other)
			return *this;

		MessageId = other.MessageId;
		ApplicationUserId = other.ApplicationUserId;
		ReactionType = other.ReactionType;
		return *this;
	}

	MessageReactionModel(MessageReactionModel&& other) noexcept
		: MessageId(std::move(other.MessageId)), ApplicationUserId(std::move(other.ApplicationUserId)), ReactionType(std::move(other.ReactionType))
	{
	}

	MessageReactionModel& operator=(MessageReactionModel&& other) noexcept
	{
		if (this == &other)
			return *this;

		MessageId = std::move(other.MessageId);
		ApplicationUserId = std::move(other.ApplicationUserId);
		ReactionType = std::move(other.ReactionType);
		return *this;
	}


	std::string GetMessageId() const { return MessageId; }
	void SetMessageId(const std::string& val) { MessageId = val; }

	std::string GetApplicationUserId() const { return ApplicationUserId; }
	void SetApplicationUserId(const std::string& val) { ApplicationUserId = val; }

	std::string GetReactionType() const { return ReactionType; }
	void SetReactionType(const std::string& val) { ReactionType = val; }



protected:
	std::string MessageId = "";
	std::string ApplicationUserId = "";
	std::string ReactionType = "";

};

