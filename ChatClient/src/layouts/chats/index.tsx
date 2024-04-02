import React from "react";
import { NavLink } from "react-router-dom";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import Navbar from "@/components/navbar";
import useLocalStorage from "@/utils/hooks/useLocalStorage";

const Chats = [
    {
        id: "1",
        name: "John Doe",
        message: "Hello, how are you?",
        time: "12:00 PM",
        avatar: "https://via.placeholder.com/150"
    },
    {
        id: "2",
        name: "John Doe",
        message: "Hello, how are you?",
        time: "12:00 PM",
        avatar: "https://via.placeholder.com/150"
    }
];


function ChatLayout({ children }: { children: React.ReactNode })
{
    const [userLocal, setUser] = useLocalStorage("user", {});

    function ChatList()
    {
        return Chats.map((chat, index) => {
            return (
                <NavLink key={index} onClick={()=> {}} className="flex items-center justify-between p-3 border-b-2" to={`/chats/${chat.id}`}>
                    <div className="flex items-center">
                        <img src={chat.avatar} alt={chat.name} className="w-12 h-12 rounded-full"/>
                        <div className="ml-3">
                            <h1 className="font-bold">{chat.name}</h1>
                            <p className="text-sm text-gray-500">{chat.message}</p>
                        </div>
                    </div>
                    <div>
                        <p className="text-sm text-gray-500">{chat.time}</p>
                    </div>
                </NavLink>
            );
        });
    }

    return (
        <div className="grid grid-cols-12 w-full h-full max-h-screen">
            <Navbar/>
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
                        {children}
                    </div>
                </div>

            </div>
        </div>
    );
}

export default ChatLayout;
