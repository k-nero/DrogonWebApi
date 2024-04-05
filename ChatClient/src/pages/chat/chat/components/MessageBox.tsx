import Message from "@/pages/chat/chat/components/Message.tsx";
import { useEffect } from "react";
import MessageType from "@/utils/type/MessageType.ts";

function MessageBox({messages} : {messages: MessageType[] | undefined })
{
    useEffect(() => {
        const messages = document.getElementById("messages");
        if(messages)
        {
            messages.scrollTop = messages.scrollHeight;
        }
    }, [messages]);

    return (
        <div className="w-full overflow-auto p-6 " id="messages">
            {
                messages?.map((message, index) => {
                    return <Message key={index} message={message} />;
                })
            }
        </div>
    );
}

export default MessageBox;

