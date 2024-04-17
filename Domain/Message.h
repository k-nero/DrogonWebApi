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

class DOMAIN_API Message : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(Message, (BaseEntity), (), (TextMessage, ApplicationUserId, ChatRoomId, QuoteMessageId,ApplicationUser, ChatRoom, MessageAttachs, MessageReactions, MessageSeenBys), ())
public:
	Message() = default;
	~Message() = default;

protected:
	std::string TextMessage = "";
	std::string ApplicationUserId = "";
	std::string ChatRoomId = "";
	std::string QuoteMessageId = "";

	std::shared_ptr<ApplicationUser> ApplicationUser = {};
	std::shared_ptr<ChatRoom> ChatRoom = {};
	std::vector<std::shared_ptr<MessageAttach>> MessageAttachs = {};
	std::vector<std::shared_ptr<MessageReaction>> MessageReactions = {};
	std::vector<std::shared_ptr<MessageSeenBy>> MessageSeenBys = {};
};

