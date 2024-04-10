import Message from "@/pages/chat/chat/components/Message.tsx";
import { useEffect, useState } from "react";
import MessageType from "@/utils/type/MessageType.ts";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { addMessageSubscriber } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";

function ScrollToBottom()
{
    const message_box = document.getElementById("message_box");
    if(message_box)
    {
        message_box.scrollTop = message_box.scrollHeight;
    }
}

function MessageBox()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [messageList, setMessageList] = useState<MessageType[]>([]);
    const [isInitLoading, setIsInitLoading] = useState<boolean>(false);
    const [isLoadMore, setIsLoadMore] = useState<boolean>(false);
    const [shouldScroll, setShouldScroll] = useState<number>(1);

    useEffect(() => {
        setIsInitLoading(true);
        Query<PaginatedType<MessageType>> (`/message?chat_id=${chat_id}&page=1&limit=30`).then((r) => {
            setMessageList(r.m_data.reverse());
            setIsInitLoading(false);
            setShouldScroll((prev) => prev + 1);
        });
    }, [chat_id]);

    useEffect(() => {
        ScrollToBottom();
    }, [shouldScroll]);

    useEffect(() => {
        addMessageSubscriber((event) => {
            const soc_mess: SocketMessageType  = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;

            if(message.ChatRoomId !== chat_id)
            {
                return;
            }

            setMessageList((prev) => {

                return [...prev, message];
            });
            if(message.ApplicationUserId === JSON.parse(localStorage.getItem("auth_credential") || "{}").user.Id)
            {
                setShouldScroll((prev) => prev + 1);
            }
        });
    }, []);

    useEffect(() => {
        if(!messageList[0])
        {
            return;
        }
         Query<PaginatedType<MessageType>>(`/message?chat_id=${chat_id}&created_date=${messageList[0].CreatedDate}&page=1&limit=30`).then((res) => {
             if(!res?.m_data)
             {
                 return;
             }
             setMessageList([...res.m_data.reverse(), ...messageList]);
             setIsLoadMore(false);
         });
    }, [isLoadMore]);

    useEffect(() => {
        const message_box = document.getElementById("message_box");
        if(message_box)
        {
            message_box.addEventListener("scroll", async () => {
                if (message_box.scrollTop === 0)
                {
                    setIsLoadMore(true);
                    message_box.scrollTop = 1;
                }
            });
        }
    }, []);

    return (
        <div className="w-full overflow-auto px-6 " id="message_box">
            {
                messageList?.map((message, index) => {
                    return <Message key={index} message={message} />;
                })
            }
        </div>
    );
}

export default MessageBox;

