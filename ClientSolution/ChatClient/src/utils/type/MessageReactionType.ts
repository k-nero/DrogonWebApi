import BaseType from "@/utils/type/BaseType.ts";
import MessageType from "./MessageType";
import ApplicationUser from "./ApplicationUser";

interface MessageReactionType extends BaseType
{
    MessageId: string;
    ApplicationUserId: string;
    ReactionType: string;
    ReactionUrl: string;
    ReactionCount: number;

    ApplicationUser: ApplicationUser;
    Message: MessageType;
}

export default MessageReactionType;
