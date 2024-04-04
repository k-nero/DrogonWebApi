import Message from "@/pages/chat/chat/components/Message.tsx";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { useEffect, useState } from "react";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import MessageType from "@/utils/type/MessageType.ts";

function MessageBox({chat_id} : {chat_id: string})
{
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;
    const [messages, setMessages] = useState<PaginatedType<MessageType>>();
    async function getMessages()
    {
        const response = await fetch(`https://192.168.50.21/api/message?chat_id=${chat_id}`, {
            method: "GET",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${credential.access_token}`
            }
        });

        if(!response.ok)
        {
            throw new Error("Failed to fetch messages");
        }
        const data = await response.json();
        setMessages(data);
        console.log(data);
    }

    useEffect(() => {
        getMessages().then(() => {});
    }, []);

    return (
        <div className="w-full overflow-auto p-6">
            {
                messages?.m_data.map((message, index) => {
                    return <Message key={index} message={message} />;
                })
            }
        </div>
    );
}

export default MessageBox;

