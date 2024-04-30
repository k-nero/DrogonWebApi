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
import {motion} from "framer-motion";

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
    const [quoteMessage, setQuoteMessage] = useState<MessageType>();

    useEffect(() => {
        localStorage.setItem("last_chat_id", chat_id);
    }, []);

    async function isOnline(user_id: string)
    {
        const res = await fetch("https://192.168.50.21:9001/online-user/" + user_id, {});
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
                    <div className={`w-full flex`}>
                        <motion.div
                            initial={false}
                            // className={`max-h-full ${isPanelOpen ? "w-[60%]" : "w-full"}`}
                            animate={{ width: isPanelOpen ? "60%" : "100%" }}
                            transition={{ duration: 0.5 }}

                        >
                            <ChatBoxHeader setIsPanel={setIsPanel} chat_room={chatRoom} onlineMap={onlineUsersMap}/>
                            <div className="px-4 flex flex-col justify-between h-[89vh]">
                                {
                                    <MessageBox messageList={messageList}
                                                setMessageList={setMessageList}
                                                setQuoteMessage={setQuoteMessage}
                                    />
                                }
                                <MessageInput messageList={messageList} quoteMessage={quoteMessage} setQuoteMessages={setQuoteMessage}/>
                            </div>
                        </motion.div>
                        {
                            <motion.div
                                initial={false}
                                animate={{

                                    width: isPanelOpen ? "40%" : "0%",
                                    display: "block",
                                    transitionEnd: {
                                        display: isPanelOpen ? "block" : "none"
                                    }
                                }}
                                transition={{ duration: 0.5 }}

                            >
                                <ChatPanel chatRoom={chatRoom} onlineMap={onlineUsersMap}/>
                            </motion.div>
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
