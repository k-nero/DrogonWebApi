import { GoDeviceCameraVideo } from "react-icons/go";
import { GrAttachment } from "react-icons/gr";
import { IoMicOutline, IoSendSharp } from "react-icons/io5";
import { FaRegClipboard } from "react-icons/fa6";
import { LuSmile } from "react-icons/lu";
import { HiDotsVertical } from "react-icons/hi";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { useState } from "react";
import { useLocation } from "react-router-dom";
const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);


function MessageInput()
{
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;

    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [message, setMessage] = useState<string>("");

    async function sendMessage()
    {
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

        if(res.ok)
        {
            setMessage("");
        }

    }

    return (
        <div className="p-8 rounded-xl m-8 bg-white">
            <div>
                <textarea placeholder="Type a message" onChange={
                    (e) => {
                        setMessage(e.target.value);
                    }
                } className="w-full bg-transparent outline-none pb-4 border-b-2 resize-none" />
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
                       <button>
                           <FaRegClipboard/>
                       </button>
                    </div>
                    <div>
                        <button className="mt-2">
                            <HiDotsVertical />
                        </button>
                    </div>
                </div>

                <div className="justify-items-end">
                    <button className="text-teal-500 font-bold" onClick={sendMessage}><IoSendSharp/></button>
                </div>

            </div>

        </div>
    );
}

export default MessageInput;
