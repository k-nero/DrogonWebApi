#pragma once
#include <boost/describe.hpp>
#include "string"
class MessageModel
{
	BOOST_DESCRIBE_CLASS(MessageModel, (), (), (TextMessage, ApplicationUserId, ChatRoomId), ())
public:
	MessageModel() = default;
	~MessageModel() = default;
	MessageModel(std::string textMessage, std::string applicationUserId, std::string chatRoomId) : TextMessage(textMessage), ApplicationUserId(applicationUserId), ChatRoomId(chatRoomId) {}
	MessageModel(const MessageModel& other) : TextMessage(other.TextMessage), ApplicationUserId(other.ApplicationUserId), ChatRoomId(other.ChatRoomId) {}
	MessageModel(MessageModel&& other) noexcept : TextMessage(std::move(other.TextMessage)), ApplicationUserId(std::move(other.ApplicationUserId)), ChatRoomId(std::move(other.ChatRoomId)) {}

	MessageModel& operator=(const MessageModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		TextMessage = other.TextMessage;
		ApplicationUserId = other.ApplicationUserId;
		ChatRoomId = other.ChatRoomId;

		return *this;
	}

	MessageModel& operator=(MessageModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		TextMessage = std::move(other.TextMessage);
		ApplicationUserId = std::move(other.ApplicationUserId);
		ChatRoomId = std::move(other.ChatRoomId);

		return *this;
	}


	std::string GetTextMessage() { return TextMessage; }
	std::string GetApplicationUserId() { return ApplicationUserId; }
	std::string GetChatRoomId() { return ChatRoomId; }
	void SetTextMessage(std::string textMessage) { TextMessage = textMessage; }
	void SetApplicationUserId(std::string applicationUserId) { ApplicationUserId = applicationUserId; }
	void SetChatRoomId(std::string chatRoomId) { ChatRoomId = chatRoomId; }
protected:
	std::string TextMessage = "";
	std::string ApplicationUserId = "";
	std::string ChatRoomId = "";

};

