#pragma once

#include <boost/describe.hpp>
#include "string"

class MessageReactionModel
{
	BOOST_DESCRIBE_CLASS(MessageReactionModel, (), (), (MessageId, ApplicationUserId, ReactionType, ReactionUrl, ReactionCount), ())
public:
	MessageReactionModel() = default;
	~MessageReactionModel() = default;

	MessageReactionModel(const std::string& messageId, const std::string& applicationUserId, const std::string& reactionType, const std::string& ReactionUrl, const int ReactionCount)
		: MessageId(messageId), ApplicationUserId(applicationUserId), ReactionType(reactionType), ReactionUrl(ReactionUrl), ReactionCount(ReactionCount)
	{
	}

	MessageReactionModel(const MessageReactionModel& other)
		: MessageId(other.MessageId), ApplicationUserId(other.ApplicationUserId), ReactionType(other.ReactionType), ReactionUrl(other.ReactionUrl), ReactionCount(other.ReactionCount)
	{
	}

	MessageReactionModel& operator=(const MessageReactionModel& other)
	{
		if (this == &other)
			return *this;

		MessageId = other.MessageId;
		ApplicationUserId = other.ApplicationUserId;
		ReactionType = other.ReactionType;
		ReactionUrl = other.ReactionUrl;
		ReactionCount = other.ReactionCount;
		return *this;
	}

	MessageReactionModel(MessageReactionModel&& other) noexcept
		: MessageId(std::move(other.MessageId)), ApplicationUserId(std::move(other.ApplicationUserId)), ReactionType(std::move(other.ReactionType)), ReactionUrl(std::move(other.ReactionUrl)), ReactionCount(std::move(other.ReactionCount))
	{
	}

	MessageReactionModel& operator=(MessageReactionModel&& other) noexcept
	{
		if (this == &other)
			return *this;

		MessageId = std::move(other.MessageId);
		ApplicationUserId = std::move(other.ApplicationUserId);
		ReactionType = std::move(other.ReactionType);
		ReactionUrl = std::move(other.ReactionUrl);
		ReactionCount = std::move(other.ReactionCount);
		return *this;
	}


	std::string GetMessageId() const { return MessageId; }
	void SetMessageId(const std::string& val) { MessageId = val; }

	std::string GetApplicationUserId() const { return ApplicationUserId; }
	void SetApplicationUserId(const std::string& val) { ApplicationUserId = val; }

	std::string GetReactionType() const { return ReactionType; }
	void SetReactionType(const std::string& val) { ReactionType = val; }

	std::string GetReactionUrl() const { return ReactionUrl; }
	void SetReactionUrl(const std::string& val) { ReactionUrl = val; }

	int GetReactionCount() const { return ReactionCount; }
	void SetReactionCount(const int val) { ReactionCount = val; }

protected:
	std::string MessageId = "";
	std::string ApplicationUserId = "";
	std::string ReactionType = "";
	std::string ReactionUrl = "";
	int ReactionCount = 0;
};

