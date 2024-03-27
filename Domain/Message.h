#pragma once
#include "BaseEntity.h"
#include "ApplicationUser.h"
#include "ChatRoom.h"
#include "memory"
#include "vector"
#include "MessageAttach.h"
#include "MessageReaction.h"
#include "MessageSeenBy.h"

class MessageAttach;
class MessageSeenBy;
class MessageReaction;

class Message : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(Message, (BaseEntity), (), (TextMessage, ApplicationUserId, ChatRoomId, ApplicationUser, ChatRoom, MessageAttachs, MessageReactions), ())
public:
	Message();
	~Message() = default;

protected:
	std::string TextMessage = "";
	std::string ApplicationUserId = "";
	std::string ChatRoomId = "";

	std::shared_ptr<ApplicationUser> ApplicationUser = {};
	std::shared_ptr<ChatRoom> ChatRoom = {};
	std::vector<std::shared_ptr<MessageAttach>> MessageAttachs = {};
	std::vector<std::shared_ptr<MessageReaction>> MessageReactions = {};
	std::vector<std::shared_ptr<MessageSeenBy>> MessageSeenBys = {};
};

