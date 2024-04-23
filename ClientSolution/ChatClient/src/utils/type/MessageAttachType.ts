import BaseType from "./BaseType";
import MessageType from "@/utils/type/MessageType.ts";

interface MessageAttachType extends BaseType
{
    AttachName: string;
    AttachUrl: string;
    MessageId: string;
    AttachType: string;
    ChatRoomId: string;

    Message: MessageType;
}

export default MessageAttachType;
