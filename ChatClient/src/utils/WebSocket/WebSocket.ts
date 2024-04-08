import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";

const auth_credential: AuthResponse = JSON.parse(localStorage.getItem("auth_credential") || "{}");

if (!auth_credential)
{
    window.location.href = "/auth/sign-in";
}

const webSocket = new WebSocket("wss://localhost:9001?authorization=" + auth_credential.access_token);
webSocket.onopen = async () => {

    const chats =  await Query<PaginatedType<ChatParticipant>>("/chat-participant");
    const chatIds: string[] = chats?.m_data?.map((chat: { ChatRoomId: string; }) => chat.ChatRoomId);

    const message  = JSON.stringify({
        type: "online",
        contacts: chatIds
    });

    webSocket.send(message);
};

webSocket.onmessage = (event) => {
    const e_data = JSON.parse(event.data);
    switch (e_data.type)
    {
        case "message":
            onMessageSubscriber.forEach((sub) => {
                sub(event);
            });
            break;
        case "typing":
            onTypingSubscriber.forEach((sub) => {
                sub(event);
            });
            break;
        case "online":
            onOnlineSubscriber.forEach((sub) => {
                sub(event);
            });
            break;
        case "offline":
            onOfflineSubscriber.forEach((sub) => {
                sub(event);
            });
            break;
    }
    console.log("WebSocket 收到消息", e_data);
};

webSocket.onclose = () => {
    console.log("WebSocket 连接关闭");
};

webSocket.onerror = (error) => {
    console.log("WebSocket 连接出错", error);
};

const onMessageSubscriber: ((event: MessageEvent) => void)[] = [];

const onTypingSubscriber: ((event: MessageEvent) => void)[] = [];

const onOnlineSubscriber: ((event: MessageEvent) => void)[] = [];

const onOfflineSubscriber: ((event: MessageEvent) => void)[] = [];

export function addMessageSubscriber(sub: (event: MessageEvent) => void)
{
    onMessageSubscriber.push(sub);
}

export function addTypingSubscriber(sub: (event: MessageEvent) => void)
{
    onTypingSubscriber.push(sub);
}

export function addOnlineSubscriber(sub: (event: MessageEvent) => void)
{
    onOnlineSubscriber.push(sub);
}

export function addOfflineSubscriber(sub: (event: MessageEvent) => void)
{
    onOfflineSubscriber.push(sub);
}

export default webSocket;
