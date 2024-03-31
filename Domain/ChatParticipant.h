#pragma once
#include "BaseEntity.h"
#include "ApplicationUser.h"
#include "ChatRoom.h"

class ApplicationUser;
class ChatRoom;

class DOMAIN_API ChatParticipant : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(ChatParticipant, (BaseEntity), (), (ChatRoomId, ApplicationUserId, ApplicationUser, ChatRoom), ())
public:
	ChatParticipant() = default;
	~ChatParticipant() = default;
protected:
	std::string ChatRoomId = "";
	std::string ApplicationUserId = "";

	std::shared_ptr<ApplicationUser> ApplicationUser = {};
	std::shared_ptr<ChatRoom> ChatRoom = {};
};

