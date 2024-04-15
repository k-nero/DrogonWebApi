import BaseType from "@/utils/type/BaseType.ts";
import MessageType from "./MessageType";
import ApplicationUser from "./ApplicationUser";

interface MessageReactionType extends BaseType
{
    MessageId: string;
    ApplicationUserId: string;
    ReactionType: string;

    ApplicationUser: ApplicationUser;
    Message: MessageType;
}

export default MessageReactionType;
