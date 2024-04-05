import ChatLayout from "@/layouts/chats";
import { NavLink, Outlet } from "react-router-dom";
import React, { useEffect, useState } from "react";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import Query from "@/utils/function/Query.ts";
import webSocket from "@/utils/function/WebSocket.ts";
import MessageType from "@/utils/type/MessageType.ts";

function ChatPage()
{
    const [chats, setchats] = useState<PaginatedType<ChatParticipant>>();

    const [messageMap, setMessageMap] = useState<Map<string, MessageType[]>>(new Map<string, MessageType[]>());

    useEffect( () => {
        Query<PaginatedType<ChatParticipant>>( "/chat-participant" ).then((r) => {
            setchats(r);
            r.m_data.map((chat) => {
                Query<PaginatedType<MessageType>>(`/message?chat_id=${chat.ChatRoomId}`).then((r) => {
                    setMessageMap((prev) => {
                        return new Map(prev.set(chat.ChatRoomId, r.m_data));
                    });
                });

               webSocket.send(JSON.stringify({
                   type: "subscribe",
                   channel: chat.ChatRoomId
               }))
            });
        });
    }, []);

    function ChatList()
    {
        if(!chats)
        {
            return <p>Loading...</p>;
        }

        return chats.m_data.map((chat, index) => {
            return (
                <NavLink key={index} onClick={()=> {}} className="flex items-center justify-between p-3 border-b-2" to={`/chats/${chat.ChatRoomId}`}>
                    <div className="flex items-center">
                        <img src={chat?.ChatRoom?.RoomImageUrl} alt={chat?.ChatRoom?.RoomName} className="w-12 h-12 rounded-full"/>
                        <div className="ml-3 truncate ">
                            <h1 className="font-bold">{chat?.ChatRoom?.RoomName}</h1>
                            <p className="text-sm text-gray-500 max-w-52">{chat.ChatRoomId}</p>
                        </div>
                    </div>
                    <div className="min-w-fit">
                        <p className="text-sm text-gray-500">{
                            new Date(chat.CreatedDate).toLocaleTimeString("en-US", {
                                hour: "2-digit",
                                minute: "2-digit"
                            })
                        }</p>
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
                                variant="auth"
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
                            <ChatList/>
                        </div>
                    </div>
                    <div className="col-span-9">
                        <Outlet context={messageMap} />
                    </div>
                </div>
            </div>
        </ChatLayout>
    )
}

export default ChatPage;

