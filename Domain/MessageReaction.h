#pragma once
#include "BaseEntity.h"
#include "Message.h"
class MessageReaction : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(MessageReaction, (BaseEntity), (), (MessageId, ApplicationUserId, ReactionType, Message, ApplicationUser), ())
public:
	MessageReaction();
	~MessageReaction() = default;

protected:
	std::string MessageId = "";
	std::string ApplicationUserId = "";
	std::string ReactionType = "";

	std::shared_ptr<Message> Message = {};
	std::shared_ptr<ApplicationUser> ApplicationUser = {};
};

