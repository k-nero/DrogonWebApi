import BaseType from "@/utils/type/BaseType.ts";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";

interface ChatRoom extends BaseType
{
    RoomName: string;
    RoomDescription: string;
    RoomImageUrl?: string;

    ChatParticipants?: ChatParticipant[];
}

export default ChatRoom;
