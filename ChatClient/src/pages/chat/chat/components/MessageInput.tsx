import { GoDeviceCameraVideo } from "react-icons/go";
import { GrAttachment } from "react-icons/gr";
import { IoMicOutline, IoSendSharp } from "react-icons/io5";
import { LuSmile } from "react-icons/lu";
import { HiDotsVertical } from "react-icons/hi";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { createRef, useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import MessageType from "@/utils/type/MessageType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function MessageInput()
{
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;
    const textMessage = createRef<HTMLTextAreaElement>();
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [typing, setTyping] = useState<boolean>(false);
    const [timer, setTimer] = useState<NodeJS.Timeout>();
    const [message, setMessage] = useState<string>("");


    useEffect(() => {
        uWebSockets.getInstance().addTypingSubscriber((event) => {
            const e_data = JSON.parse(event.data);
            if (e_data.channel === chat_id)
            {
                setTyping(e_data.typing);
            }
        });
    }, [chat_id]);


    async function sendMessage()
    {
        if(uWebSockets.getInstance().ConnectState() !== 1)
        {
            alert("Connection lost, reconnecting...");
        }

        if (!message)
        {
            return;
        }

        const res = await fetch(`${baseUrl}/message`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${user.access_token}`
            },
            body: JSON.stringify({
                TextMessage: message,
                ChatRoomId: chat_id,
                ApplicationUserId: user.user.Id
            })
        });

        if (res.ok)
        {

            setMessage("");
            const rs = await res.json();
            const message = await Query<MessageType>(`/message/${rs.id}`);
            uWebSockets.getInstance().send(JSON.stringify({
                type: "message",
                channel: chat_id,
                message: message
            }));
        }

    }

    function handleTyping(e)
    {
        if (!typing && e.target.value != "")
        {
            setTyping(true);
            uWebSockets.getInstance().send(JSON.stringify({
                type: "typing",
                channel: chat_id,
                typing: true
            }));
        }

        clearTimeout(timer);
        setTimer(setTimeout(() => {
            setTyping(false);
            uWebSockets.getInstance().send(JSON.stringify({
                type: "typing",
                channel: chat_id,
                typing: false
            }));
        }, 1500));
    }

    return (
        <div>

            <div className={`flex items-center justify-center text-teal-500 ${!typing ? "hidden" : "" }`}>
                <div className="animate-bounce w-3 h-3 bg-teal-500 rounded-full"></div>
                <p className="ml-2">Someone is typing...</p>
            </div>

            <div className="p-8 rounded-xl mx-8 mt-4 bg-white">
                <form onSubmit={(e) => {
                    e.preventDefault();
                    e.currentTarget.reset();
                    sendMessage().then(() => {});
                }}>
                    <div>
                <textarea placeholder="Type a message" ref={textMessage} onChange={
                    (e) => {
                        setMessage(e.target.value);
                        handleTyping(e);
                    }
                } onKeyDown={(event) => {

                    if (event.key === "Enter")
                    {
                        event.preventDefault();
                        sendMessage().then(() => {

                        });
                        event.currentTarget.value = "";
                    }
                    if (event.key === "Enter" && event.shiftKey)
                    {
                        event.currentTarget.value += "\n";
                    }

                }} className="w-full bg-transparent outline-none pb-4 border-b-2 resize-none"/>
                    </div>
                    <div className="flex justify-between text-2xl mt-4">
                        <div className="flex gap-3">
                            <div className="flex gap-3 pr-3 border-r-2">
                                <button>
                                    <GoDeviceCameraVideo/>
                                </button>
                                <button>
                                    <IoMicOutline/>
                                </button>
                            </div>
                            <div className="flex gap-3 pr-3 border-r-2">
                                <button>
                                    <LuSmile/>
                                </button>
                                <button>
                                    <GrAttachment/>
                                </button>
                            </div>
                            <div>
                                <button className="mt-2">
                                    <HiDotsVertical/>
                                </button>
                            </div>
                        </div>

                        <div className="justify-items-end">
                            <button className="text-teal-500 font-bold" type="submit"><IoSendSharp/></button>
                        </div>
                    </div>
                </form>
            </div>
        </div>

    );
}

export default MessageInput;
