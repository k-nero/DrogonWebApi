#pragma once
#include "BaseEntity.h"
#include "ChatParticipant.h"
#include <memory>
#include <vector>

class ChatParticipant;
class ApplicationUser;
class Message;

class DOMAIN_API ChatRoom : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(ChatRoom, (BaseEntity), (), (RoomName, RoomDescription, RoomImageUrl, ChatParticipants), ())
public:
	ChatRoom() = default;
	~ChatRoom() = default;

protected:
	std::string RoomName = "";
	std::string RoomDescription = "";
	std::string RoomImageUrl = "";

	std::vector<std::shared_ptr<ChatParticipant>> ChatParticipants = {};
};

