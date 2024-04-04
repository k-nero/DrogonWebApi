import ApplicationUser from "@/utils/type/ApplicationUser.ts";
import BaseType from "@/utils/type/BaseType.ts";
import ChatRoom from "./ChatRoom";

interface ChatParticipant extends BaseType
{
    ChatRoomId: string,
    ApplicationUserId: string,

    ApplicationUser?: ApplicationUser,
    ChatRoom?: ChatRoom,
}

export default ChatParticipant;
