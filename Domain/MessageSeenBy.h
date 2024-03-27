#pragma once
#include "BaseEntity.h"
#include "ApplicationUser.h"
#include "Message.h"


class MessageSeenBy : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(MessageSeenBy, (BaseEntity), (), (MessageId, ApplicationUserId, Message, ApplicationUser), ())
public:
	MessageSeenBy();
	~MessageSeenBy() = default;
protected:
	std::string MessageId = "";
	std::string ApplicationUserId = "";

	std::shared_ptr<Message> Message = {};
	std::shared_ptr<ApplicationUser> ApplicationUser = {};
};

