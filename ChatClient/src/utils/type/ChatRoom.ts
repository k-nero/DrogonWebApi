import BaseType from "@/utils/type/BaseType.ts";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);



interface ChatRoom extends BaseType
{
    RoomName: string;
    RoomDescription: string;
    RoomImageUrl?: string;

    ChatParticipants?: ChatParticipant[];
}

async function FetchChats()
{
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;
    const res = await fetch(`${baseUrl}/chat-participant`, {
        method: "GET",
        headers: {
            "Content-Type": "application/json",
            "Authorization": `Bearer ${credential.access_token}`
        }
    });

    if(!res.ok)
    {
        throw new Error("Failed to fetch chats");
    }
    const data : PaginatedType<ChatParticipant> = await res.json();
    return data;
}

export { FetchChats };
export type { ChatRoom };

