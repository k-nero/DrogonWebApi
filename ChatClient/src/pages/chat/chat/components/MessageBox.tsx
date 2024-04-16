import Message from "@/pages/chat/chat/components/Message.tsx";
import { Dispatch, SetStateAction, useEffect, useState } from "react";
import MessageType from "@/utils/type/MessageType.ts";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";
import SocketMessage from "@/utils/WebSocket/SocketMessage.ts";
import MessageReactionType from "@/utils/type/MessageReactionType.ts";
import { Guid } from "guid-typescript";

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function ScrollToBottom()
{
    const message_box = document.getElementById("message_box");
    if (message_box)
    {
        message_box.scrollTop = message_box.scrollHeight;
    }
}

function MessageBox({ messageList, setMessageList }: {
    messageList: MessageType[],
    setMessageList: Dispatch<SetStateAction<MessageType[]>>
})
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;

    const [isInitLoading, setIsInitLoading] = useState<boolean>(false);
    const [shouldLoadMore, setShouldLoadMore] = useState<boolean>(false);
    const [shouldScroll, setShouldScroll] = useState<boolean>(false);
    const [isLoadFinished, setIsLoadFinished] = useState<boolean>(false);


    useEffect(() => {
        setIsInitLoading(true);
        Query<PaginatedType<MessageType>>(`/message?chat_id=${chat_id}&page=1&limit=30`).then((r) => {
            setMessageList(r.m_data.reverse());
            setIsInitLoading(false);
            setShouldScroll(prev => !prev);
            setIsLoadFinished(true);
        });
    }, [chat_id]);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSeenBySubscriber((event) => {
            const e_data = JSON.parse(event.data);
            setMessageList((prev) => {
                return prev.map((message) => {
                    if (message.Id === e_data.message.MessageId)
                    {
                        message.MessageSeenBys?.push(e_data.message);
                    }
                    return message;
                });
            });
        });
    }, []);

    useEffect(() => {
        messageList.reverse().map((message) => {
            if (message.ApplicationUserId !== user.user.Id)
            {
                if (message.MessageSeenBys?.findIndex((seenBy) => seenBy.ApplicationUserId === user.user.Id) === -1)
                {

                    fetch(`${baseUrl}/message-seen-by`, {
                        method: "POST",
                        headers: {
                            "Content-Type": "application/json"
                        },
                        body: JSON.stringify(
                            {
                                ApplicationUserId: user.user.Id,
                                MessageId: message.Id
                            }
                        )
                    }).then(r => {
                        if (r.ok)
                        {
                            r.json().then((rs) => {
                                Query<MessageSeenByType>(`/message-seen-by/${rs.id}`).then((r) => {

                                    setMessageList((prev) => {
                                        return prev.map((m) => {
                                            if (m.Id === message.Id)
                                            {
                                                m.MessageSeenBys?.push(r);
                                            }
                                            return m;
                                        });
                                    });

                                    uWebSockets.getInstance().send(JSON.stringify({
                                        type: "message_seen_by",
                                        channel: chat_id,
                                        message: r
                                    }));
                                });
                            });
                        }
                    });
                }
            }
        });
        messageList.reverse();
    }, [isLoadFinished]);

    useEffect(() => {
        ScrollToBottom();
    }, [shouldScroll]);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSubscriber((event) => {
            const soc_mess: SocketMessageType = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;
            if (message.ChatRoomId !== chat_id)
            {
                return;
            }
            setMessageList((prev) => {
                return [...prev, message];
            });
            const message_box = document.getElementById("message_box");
            if (message.ApplicationUserId === user.user.Id)
            {
                setShouldScroll(prev => !prev);
            }
            else if (message_box && message_box.scrollTop >= message_box.scrollHeight - message_box.clientHeight - 200)
            {
                setShouldScroll(prev => !prev);
            }

        });
    }, []);

    useEffect(() => {
        if (!messageList[0])
        {
            return;
        }

        setTimeout(() => {
            Query<PaginatedType<MessageType>>(`/message?chat_id=${chat_id}&created_date=${messageList[0].CreatedDate}&page=1&limit=30`).then((res) => {
                if (!res?.m_data)
                {
                    return;
                }
                setMessageList([...res.m_data.reverse(), ...messageList]);
            });
        }, 500);


    }, [shouldLoadMore]);

    useEffect(() => {
        uWebSockets.getInstance().addReactionSubscriber((event) => {
            const e_data: SocketMessage<MessageReactionType> = JSON.parse(event.data);
            if (e_data.channel === chat_id)
            {
                setMessageList((prev) => {
                    return prev.map((message) => {
                        if (message.Id === e_data.message.MessageId)
                        {
                            message.MessageReactions?.push(e_data.message);
                        }
                        return message;
                    });
                });
            }
        });
    }, []);

    useEffect(() => {
        const message_box = document.getElementById("message_box");
        if (message_box)
        {
            message_box.addEventListener("scroll", () => {
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

                    const currentMessageDate = new Date(message.CreatedDate);
                    const nextMessageDate = new Date(messageList[index + 1]?.CreatedDate);

                    const offset = nextMessageDate.getTime() - currentMessageDate.getTime();
                    return (
                        <div key={Guid.create().toString()}>
                            <Message message={message} showTime={true}/>
                            {
                                offset > 1000 * 60 * 30 ?
                                    <div className="text-center text-gray-500 text-xs"> {new Date(message.CreatedDate).toLocaleString("en-US", {
                                        month: "short",
                                        day: "numeric",
                                        year: "numeric",
                                        hour: "numeric",
                                        minute: "numeric",
                                        hour12: false
                                    })} </div>
                                    : null
                            }

                        </div>
                    );
                })
            }
        </div>
    );
}

export default MessageBox;

