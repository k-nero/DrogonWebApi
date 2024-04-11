import Message from "@/pages/chat/chat/components/Message.tsx";
import { useEffect, useState } from "react";
import MessageType from "@/utils/type/MessageType.ts";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";

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
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;

    const [messageList, setMessageList] = useState<MessageType[]>([]);
    const [isInitLoading, setIsInitLoading] = useState<boolean>(false);
    const [shouldLoadMore, setShouldLoadMore] = useState<boolean>(false);
    const [shouldScroll, setShouldScroll] = useState<boolean>(false);

    useEffect(() => {
        setIsInitLoading(true);
        Query<PaginatedType<MessageType>> (`/message?chat_id=${chat_id}&page=1&limit=30`).then((r) => {
            setMessageList(r.m_data.reverse());
            setIsInitLoading(false);
            setShouldScroll(prev => !prev);
        });
    }, [chat_id]);

    useEffect(() => {
        ScrollToBottom();
    }, [shouldScroll]);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSubscriber((event) => {
            const soc_mess: SocketMessageType  = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;
            if(message.ChatRoomId !== chat_id)
            {
                return;
            }
            setMessageList((prev) => {
                return [...prev, message];
            });
            if(message.ApplicationUserId === user.user.Id)
            {
                setShouldScroll(prev => !prev);
            }
        });
    }, []);

    useEffect(() => {
        if(!messageList[0])
        {
            return;
        }

        setTimeout(() => {
            Query<PaginatedType<MessageType>>(`/message?chat_id=${chat_id}&created_date=${messageList[0].CreatedDate}&page=1&limit=30`).then((res) => {
                if(!res?.m_data)
                {
                    return;
                }
                setMessageList([...res.m_data.reverse(), ...messageList]);
            });
        }, 500);


    }, [shouldLoadMore]);

    useEffect(() => {
        const message_box = document.getElementById("message_box");
        if(message_box)
        {
            message_box.addEventListener("scroll",  () => {
                if (message_box.scrollTop === 0)
                {
                    message_box.scrollTop = 5;
                    setShouldLoadMore(prevState => !prevState);
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

