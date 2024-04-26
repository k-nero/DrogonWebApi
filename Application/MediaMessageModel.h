#pragma once
#include <string>
#include <boost/describe.hpp>
#include <vector>
#include "MessageAttachModel.h"

class MediaMessageModel
{
	BOOST_DESCRIBE_CLASS(MediaMessageModel, (), (), (TextMessage, ApplicationUserId, ChatRoomId, QuoteMessageId, Media), ())
public:
	MediaMessageModel() = default;
	~MediaMessageModel() = default;
	MediaMessageModel(std::string textMessage, std::string applicationUserId, std::string chatRoomId, std::string quoteMessageId) : TextMessage(textMessage), ApplicationUserId(applicationUserId), ChatRoomId(chatRoomId), QuoteMessageId(quoteMessageId) {}
	MediaMessageModel(const MediaMessageModel& other) : TextMessage(other.TextMessage), ApplicationUserId(other.ApplicationUserId), ChatRoomId(other.ChatRoomId), QuoteMessageId(other.QuoteMessageId), Media(other.Media) {}
	MediaMessageModel(MediaMessageModel&& other) noexcept : TextMessage(std::move(other.TextMessage)), ApplicationUserId(std::move(other.ApplicationUserId)), ChatRoomId(std::move(other.ChatRoomId)), QuoteMessageId(std::move(other.QuoteMessageId)), Media(std::move(other.Media)) {}

	MediaMessageModel& operator=(const MediaMessageModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		TextMessage = other.TextMessage;
		ApplicationUserId = other.ApplicationUserId;
		ChatRoomId = other.ChatRoomId;
		QuoteMessageId = other.QuoteMessageId;
		Media = other.Media;

		return *this;
	}

	MediaMessageModel& operator=(MediaMessageModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		TextMessage = std::move(other.TextMessage);
		ApplicationUserId = std::move(other.ApplicationUserId);
		ChatRoomId = std::move(other.ChatRoomId);
		QuoteMessageId = std::move(other.QuoteMessageId);
		Media = std::move(other.Media);

		return *this;
	}


	std::string GetTextMessage() { return TextMessage; }
	std::string GetApplicationUserId() { return ApplicationUserId; }
	std::string GetChatRoomId() { return ChatRoomId; }
	std::string GetQuoteMessageId() { return QuoteMessageId; }
	std::vector<MessageAttachModel> GetMedia() { return Media; }
	void SetTextMessage(std::string textMessage) { TextMessage = textMessage; }
	void SetApplicationUserId(std::string applicationUserId) { ApplicationUserId = applicationUserId; }
	void SetChatRoomId(std::string chatRoomId) { ChatRoomId = chatRoomId; }
	void SetQuoteMessageId(std::string quoteMessageId) { QuoteMessageId = quoteMessageId; }
	void SetMedia(std::vector<MessageAttachModel> media) { Media = media; }
protected:
	std::string TextMessage = "";
	std::string ApplicationUserId = "";
	std::string ChatRoomId = "";
	std::string QuoteMessageId = "";

	std::vector<MessageAttachModel> Media = {};
};