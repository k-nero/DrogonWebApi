import { useLocation } from "react-router-dom";
import React, {  useEffect, useState } from "react";
import ChatBoxHeader from "@/pages/chat/chat/components/ChatBoxHeader.tsx";
import MessageBox from "@/pages/chat/chat/components/MessageBox.tsx";
import MessageInput from "@/pages/chat/chat/components/MessageInput.tsx";
import ChatPanel from "@/pages/chat/chat/components/ChatPanel.tsx";
import ChatRoom from "@/utils/type/ChatRoom.ts";
import Query from "@/utils/function/Query.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import MessageType from "@/utils/type/MessageType.ts";

function Chat()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;

    //states
    const [isPanelOpen, setIsPanelOpen] = useState(false);
    const [chatRoom, setChatRoom] = useState<ChatRoom>();
    const [onlineUsersMap, setOnlineUsersMap] = useState<Map<string, boolean>>(new Map<string, boolean>());
    const [messageList, setMessageList] = useState<MessageType[]>([]);

    async function isOnline(user_id: string)
    {
        const res = await fetch("https://localhost:9001/online-user/" + user_id, {});
        const online = await res.text();
        return online === "true";
    }

    useEffect(() => {
        Query<ChatRoom>(`/chat-room/${chat_id}`).then((r) => {
            setChatRoom(r);
            r?.ChatParticipants?.map((participant) => {
                if (participant.ApplicationUserId === credential.user.Id)
                {
                    return;
                }
                isOnline(participant.ApplicationUserId).then((online) => {
                    setOnlineUsersMap((prev) => {
                        return new Map(prev.set(participant.ApplicationUserId, online));
                    });
                });
            });
        });
    }, [chat_id, credential.user.Id]);


    useEffect(() => {
        uWebSockets.getInstance().addOnlineSubscriber((event) => {
            const e_data = JSON.parse(event.data);
            setOnlineUsersMap((prev) => {
                return new Map(prev.set(e_data.socket_id, true));
            });
        });

        uWebSockets.getInstance().addOfflineSubscriber((event) => {
            const e_data = JSON.parse(event.data);
            setOnlineUsersMap((prev) => {
                return new Map(prev.set(e_data.socket_id, false));
            });
        });
    }, []);

    function setIsPanel()
    {
        setIsPanelOpen(!isPanelOpen);
    }

    return (
        <>
            {
                chat_id ?
                    <div className={`w-full h-fit ${isPanelOpen ? "grid grid-cols-10" : ""}`}>
                        <div className={`w-full h-fit ${isPanelOpen ? "col-span-6" : ""}`}>
                            <ChatBoxHeader setIsPanel={setIsPanel} chat_room={chatRoom} onlineMap={onlineUsersMap}/>
                            <div className="bg-gray-100 bg-opacity-90 p-4 flex flex-col justify-between h-[90vh]">
                                <MessageBox messageList={messageList} setMessageList={setMessageList}/>
                                <MessageInput messageList={messageList} setMessageList={setMessageList}/>
                            </div>
                        </div>
                        {
                            isPanelOpen &&
                            <div className="col-span-4 ">
                                <ChatPanel setIsPanel={setIsPanel}/>
                            </div>
                        }
                    </div>
                    :
                    <div className="flex items-center justify-center h-full">
                        <h1 className="text-2xl ">Select a chat to start conversion </h1>
                    </div>
            }
        </>
    );
}

export default Chat;
