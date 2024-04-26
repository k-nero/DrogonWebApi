import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";
const socket = new URL(`${import.meta.env.VITE_WEBSOCKET_URL}`);


export class uWebSockets
{
    private static webSocket: uWebSockets;
    private ws: WebSocket;

    private onMessageSubscriber: ((event: MessageEvent) => void)[] = [];
    private onTypingSubscriber: ((event: MessageEvent) => void)[] = [];
    private onOnlineSubscriber: ((event: MessageEvent) => void)[] = [];
    private onOfflineSubscriber: ((event: MessageEvent) => void)[] = [];
    private onMessageSeenBySubscriber: ((event: MessageEvent) => void)[] = [];
    private onReactionSubscriber: ((event: MessageEvent) => void)[] = [];

    public ConnectState()
    {
        return this.ws.readyState;
    }

    private initWebSocket()
    {
        this.ws.onopen = async () => {

            const chats = await Query<PaginatedType<ChatParticipant>>("/chat-participant");
            const chatIds: string[] = chats?.m_data?.map((chat: { ChatRoomId: string; }) => chat.ChatRoomId);

            //TODO: add check if user hide online status

            const message = JSON.stringify({
                type: "online",
                contacts: chatIds
            });

            this.ws.send(message);
        };

        this.ws.onmessage = (event) => {
            const e_data = JSON.parse(event.data);
            switch (e_data.type)
            {
                case "message":
                    this.onMessageSubscriber.forEach((sub) => {
                        sub(event);
                    });
                    break;
                case "typing":
                    this.onTypingSubscriber.forEach((sub) => {
                        sub(event);
                    });
                    break;
                case "online":
                    this.onOnlineSubscriber.forEach((sub) => {
                        sub(event);
                    });
                    break;
                case "offline":
                    this.onOfflineSubscriber.forEach((sub) => {
                        sub(event);
                    });
                    break;
                case "message_seen_by":
                    this.onMessageSeenBySubscriber.forEach((sub) => {
                        sub(event);
                    });
                    break;
                case "reaction":
                    this.onReactionSubscriber.forEach((sub) => {
                        sub(event);
                    });
                    break;
            }
            console.log("WebSocket 收到消息", e_data);
        };

        this.ws.onclose = () => {
            console.log("WebSocket 连接关闭");
            console.log("Attempting to reconnect...");
            setTimeout(() => {
                this.ws = new WebSocket( socket + "?authorization=" + JSON.parse(localStorage.getItem("auth_credential") || "{}").access_token);
                this.initWebSocket();
                window.location.reload();
            }, 5000);
        };

        this.ws.onerror = (error) => {
            console.log("WebSocket 连接出错", error);
            console.log("Attempting to reconnect...");
            setTimeout(() => {
                this.ws = new WebSocket(socket + "?authorization=" + JSON.parse(localStorage.getItem("auth_credential") || "{}").access_token);
                this.initWebSocket();
                window.location.reload();
            }, 5000);
        };
    }

    private constructor()
    {
        const auth_credential: AuthResponse = JSON.parse(localStorage.getItem("auth_credential") || "{}");
        this.ws = new WebSocket(socket + "?authorization=" + auth_credential.access_token);
        this.initWebSocket();
    }

    public static getInstance()
    {
        if (!uWebSockets.webSocket)
        {
            uWebSockets.webSocket = new uWebSockets();
        }
        return uWebSockets.webSocket;
    }

    public addMessageSubscriber(sub: (event: MessageEvent) => void)
    {
        this.onMessageSubscriber.push(sub);
    }

    public addTypingSubscriber(sub: (event: MessageEvent) => void)
    {
        this.onTypingSubscriber.push(sub);
    }

    public addOnlineSubscriber(sub: (event: MessageEvent) => void)
    {
        this.onOnlineSubscriber.push(sub);
    }

    public addOfflineSubscriber(sub: (event: MessageEvent) => void)
    {
        this.onOfflineSubscriber.push(sub);
    }

    public addMessageSeenBySubscriber(sub: (event: MessageEvent) => void)
    {
        this.onMessageSeenBySubscriber.push(sub);
    }

    public addReactionSubscriber(sub: (event: MessageEvent) => void)
    {
        this.onReactionSubscriber.push(sub);
    }

    public send(message: string)
    {
        this.ws.send(message);
    }
}
