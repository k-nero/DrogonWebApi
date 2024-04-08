import { useLocation, useOutletContext } from "react-router-dom";
import { useEffect, useState } from "react";
import ChatBoxHeader from "@/pages/chat/chat/components/ChatBoxHeader.tsx";
import MessageBox from "@/pages/chat/chat/components/MessageBox.tsx";
import MessageInput from "@/pages/chat/chat/components/MessageInput.tsx";
import ChatPanel from "@/pages/chat/chat/components/ChatPanel.tsx";
import MessageType from "@/utils/type/MessageType.ts";
import ChatRoom from "@/utils/type/ChatRoom.ts";
import Query from "@/utils/function/Query.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { addOfflineSubscriber, addOnlineSubscriber } from "@/utils/WebSocket/WebSocket.ts";

function Chat()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [isPanelOpen, setIsPanelOpen] = useState(false);
    const messageMap: Map<string, MessageType[]> = useOutletContext();
    const [chatRoom, setChatRoom] = useState<ChatRoom>();
    const [onlineUsersMap, setOnlineUsersMap] = useState<Map<string, boolean>>(new Map<string, boolean>());

    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;

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
    }, []);


    useEffect(() => {
        addOnlineSubscriber((event) => {
            const e_data = JSON.parse(event.data);
            setOnlineUsersMap((prev) => {
                return new Map(prev.set(e_data.socket_id, true));
            });
        });

        addOfflineSubscriber((event) => {
            const e_data = JSON.parse(event.data);
            console.log(e_data);
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
                                <MessageBox messages={messageMap.get(chat_id)}/>
                                <MessageInput/>
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
