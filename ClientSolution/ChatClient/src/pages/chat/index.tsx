import ChatLayout from "@/layouts/chats";
import { NavLink, useNavigate, useOutlet } from "react-router-dom";
import React, { useEffect, useState } from "react";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import Query from "@/utils/function/Query.ts";
import MessageType from "@/utils/type/MessageType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import SocketMessage from "@/utils/WebSocket/SocketMessage.ts";
import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";
import { GrAttachment } from "react-icons/gr";
import { Skeleton } from "antd";

function ChatPage()
{
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;
    const outlet = useOutlet();

    const [chats, setChats] = useState<PaginatedType<ChatParticipant>>();
    const [messageMap, setMessageMap] = useState<Map<string, MessageType[]>>(new Map<string, MessageType[]>());

    const navigate = useNavigate();


    useEffect(() => {
        const last_chat_id = localStorage.getItem("last_chat_id");
        if(last_chat_id)
        {
            navigate(`/chats/${last_chat_id}`);
        }
    }, []);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSubscriber((event) => {
            const soc_mess: SocketMessage<MessageType> = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;
            setMessageMap((prev) => {
                return new Map(prev.set(message.ChatRoomId, [...prev.get(message.ChatRoomId) || [], message]));
            });
        });

        Query<PaginatedType<ChatParticipant>>("/chat-participant").then((r) => {
            setChats(r);
            r?.m_data?.map((chat) => {
                Query<PaginatedType<MessageType>>(`/message?chat_id=${chat.ChatRoomId}&page=1&limit=10`).then((r) => {
                    setMessageMap((prev) => {
                        return new Map(prev.set(chat.ChatRoomId, r?.m_data.reverse()));
                    });
                });
                uWebSockets.getInstance().send(JSON.stringify({
                    type: "subscribe",
                    channel: chat.ChatRoomId
                }));
            });
        });
    }, []);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSeenBySubscriber((event) => {
            const e_data: SocketMessage<MessageSeenByType> = JSON.parse(event.data);
            setMessageMap((prev) => {
                const prev_state = prev.get(e_data.channel);
                if (!prev_state)
                {
                    return prev;
                }
                return new Map(prev.set(e_data.channel, prev_state.map((message) => {
                    if (message.Id === e_data.message.MessageId)
                    {
                        message.MessageSeenBys?.push(e_data.message);
                    }
                    return message;
                })));
            });
        });
    }, []);

    function ChatList()
    {
        if(!chats)
        {
         return <Skeleton active avatar/>
        }

        return chats?.m_data.map((chat, index) => {

            const message = messageMap.get(chat.ChatRoomId);
            const lastMessage = message?.[message.length - 1];
            const unreadMessage = message?.filter((m) => m.ApplicationUserId !== user.user.Id && !m.MessageSeenBys?.find((seen) => seen.ApplicationUserId === user.user.Id)).length || 0;

            return (
                <NavLink key={index} onClick={() => {}} className={`flex items-center justify-between p-3 ${index === (chats.m_data.length - 1) ? "" : "border-b-2"}`} to={`/chats/${chat.ChatRoomId}`}>
                    <div className="flex items-center">
                        <img loading="lazy" src={chat?.ChatRoom?.RoomImageUrl} alt={chat?.ChatRoom?.RoomName} className="w-12 h-12 rounded-full"/>
                        <div className="ml-3 truncate ">
                            <h1 className="font-bold">{chat?.ChatRoom?.RoomName}</h1>
                            <div className={`${unreadMessage > 0 ? "font-bold" : ""} text-sm text-gray-500 max-w-52`}>

                                {
                                    message ?
                                    lastMessage?.TextMessage ? lastMessage.TextMessage : lastMessage?.MessageAttachs?.length || 0 > 0 ? ( <>
                                        <GrAttachment className="inline-block"/>
                                        [Attachment]
                                    </> ) : null : <Skeleton.Input style={{height: "16px"}}  active/>
                                }
                            </div>
                        </div>
                    </div>
                    <div className="min-w-fit">
                        <p className="text-sm text-gray-500">{
                            new Date(lastMessage?.CreatedDate || chat.CreatedDate).toLocaleTimeString("en-US", {
                                hour: "2-digit",
                                minute: "2-digit"
                            })
                        }</p>
                        {
                            unreadMessage ?
                                <p className="bg-red-500 rounded-xl text-sm text-white w-6 m-auto text-center">
                                    {unreadMessage > 9 ? "9+" : unreadMessage}
                                </p>
                                : null
                        }

                    </div>
                </NavLink>
            );
        });
    }

    return (
        <ChatLayout>
            <div className="min-h-screen col-span-11">
                <div className="grid grid-cols-12">
                    <div className="p-5 col-span-3 min-h-screen border-r-2">
                        <div className="flex justify-between">
                            <h1 className="font-bold text-2xl">Messages</h1>
                            <button>
                                <BsThreeDots className="text-2xl opacity-50"/>
                            </button>
                        </div>
                        <div className="">
                            <InputField
                                variant=""
                                extra="mb-3"
                                placeholder="Search for chat"
                                type="text"
                                label={""}
                                id={""}
                            />
                        </div>
                        <div>
                            <button className="text-center bg-teal-500 w-full p-4 rounded-full text-white font-bold">
                                Start a new chat
                            </button>
                        </div>
                        <div className="mt-4">
                            {
                                <ChatList/>
                            }

                        </div>
                    </div>
                    <div className="col-span-9 bg-gray-100 bg-opacity-90">
                        {
                            outlet || <p>Select a chat to start conversion</p>
                        }
                    </div>
                </div>
            </div>
        </ChatLayout>
    );
}

export default ChatPage;

