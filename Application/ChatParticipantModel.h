#pragma 
#include <boost/describe.hpp>
#include <string>

class ChatParticipantModel
{
	BOOST_DESCRIBE_CLASS(ChatParticipantModel, (), (), (ChatRoomId, ApplicationUserId), ())
public:
	ChatParticipantModel() = default;
	~ChatParticipantModel() = default;

	ChatParticipantModel(std::string chatRoomId, std::string applicationUserId) : ChatRoomId(chatRoomId), ApplicationUserId(applicationUserId)
	{

	}

	ChatParticipantModel(const ChatParticipantModel& other) : ChatRoomId(other.ChatRoomId), ApplicationUserId(other.ApplicationUserId)
	{

	}

	ChatParticipantModel(ChatParticipantModel&& other) noexcept : ChatRoomId(std::move(other.ChatRoomId)), ApplicationUserId(std::move(other.ApplicationUserId))
	{

	}

	ChatParticipantModel& operator=(const ChatParticipantModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		ChatRoomId = other.ChatRoomId;
		ApplicationUserId = other.ApplicationUserId;

		return *this;
	}

	ChatParticipantModel& operator=(ChatParticipantModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		ChatRoomId = std::move(other.ChatRoomId);
		ApplicationUserId = std::move(other.ApplicationUserId);

		return *this;
	}

	std::string GetChatRoomId() const
	{
		return ChatRoomId;
	}

	std::string GetApplicationUserId() const
	{
		return ApplicationUserId;
	}

	void SetChatRoomId(std::string chatRoomId)
	{
		ChatRoomId = chatRoomId;
	}

	void SetApplicationUserId(std::string applicationUserId)
	{
		ApplicationUserId = applicationUserId;
	}
protected:
	std::string ChatRoomId = "";
	std::string ApplicationUserId = "";
};

