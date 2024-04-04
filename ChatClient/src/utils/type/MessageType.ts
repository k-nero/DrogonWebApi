import BaseType from "@/utils/type/BaseType.ts";
import ChatRoom from "./ChatRoom";
import ApplicationUser from "./ApplicationUser";

interface MessageType extends BaseType
{
    TextMessage: string;
    ApplicationUserId: string;
    ChatRoomId: string;

    ChatRoom?: ChatRoom;
    ApplicationUser?: ApplicationUser;
}

export default MessageType;
