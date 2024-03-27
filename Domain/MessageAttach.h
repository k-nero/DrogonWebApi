#pragma once
#include "BaseEntity.h"
#include "Message.h"
#include <memory>

class Message;

class MessageAttach : public BaseEntity
{
	BOOST_DESCRIBE_CLASS(MessageAttach, (BaseEntity), (), (AttachName, AttactUrl, MessageId, AttachType, Message), ())
public:
	MessageAttach();
	~MessageAttach() = default;
protected:
	std::string AttachName = "";
	std::string AttactUrl = "";
	std::string MessageId = "";
	std::string AttachType = "";

	std::shared_ptr<Message> Message = {};
};

