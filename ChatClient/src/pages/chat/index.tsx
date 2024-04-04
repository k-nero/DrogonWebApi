import ChatLayout from "@/layouts/chats";
import { NavLink, Outlet } from "react-router-dom";
import React from "react";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function ChatPage()
{
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;
    const [chats, setchats] = React.useState<PaginatedType<ChatParticipant>>();

    async function FetchChats()
    {
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
        console.log(data);
        setchats(data);
    }

    React.useEffect( () => {
        FetchChats().then(() => {});
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
                        <Outlet/>
                    </div>
                </div>
            </div>
        </ChatLayout>
    )
}

export default ChatPage;
