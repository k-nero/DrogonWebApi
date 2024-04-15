import BaseType from "@/utils/type/BaseType.ts";
import ChatRoom from "./ChatRoom";
import ApplicationUser from "./ApplicationUser";
import MessageReactionType from "@/utils/type/MessageReactionType.ts";
import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";

interface MessageType extends BaseType
{
    TextMessage: string;
    ApplicationUserId: string;
    ChatRoomId: string;

    ChatRoom?: ChatRoom;
    ApplicationUser?: ApplicationUser;
    MessageReactions?: MessageReactionType[];
    MessageSeenBys?: MessageSeenByType[];
    MessageAttachs?: MessageAttachType[];
}

export default MessageType;
