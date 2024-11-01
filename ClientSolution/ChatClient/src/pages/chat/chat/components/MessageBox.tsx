import { MemoizedMessage } from "@/pages/chat/chat/components/Message.tsx";
import React, { Dispatch, SetStateAction, useEffect, useRef, useState } from "react";
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

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function MessageBox({ messageList, setMessageList, setQuoteMessage }: {
    messageList: MessageType[],
    setMessageList: Dispatch<SetStateAction<MessageType[]>>,
    setQuoteMessage: Dispatch<SetStateAction<MessageType | undefined>>,
})
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;

    const [shouldLoadMore, setShouldLoadMore] = useState<boolean>(false);
    const [shouldScroll, setShouldScroll] = useState<boolean>(false);
    const [isAtTop, setIsAtTop] = useState<boolean>(false);
    const [isLoadFinished, setIsLoadFinished] = useState<boolean>(false);
    const message_box = useRef(null as HTMLDivElement | null)
    useEffect(() => {
        Query<PaginatedType<MessageType>>(`/message?chat_id=${chat_id}&page=1&limit=30`).then((r) => {
            setMessageList(r.m_data);
            setShouldScroll(true);
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
        if (message_box.current)
        {
            message_box.current.scrollTop = message_box.current.scrollHeight;
        }
    }, [shouldScroll]);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSubscriber(async (event) => {
            const soc_mess: SocketMessageType = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;
            if (message.ChatRoomId !== chat_id)
            {
                return;
            }
            setMessageList((prev) => {
                return [message, ...prev];
            });
            if (message.ApplicationUserId === user.user.Id)
            {
                await new Promise((r) => setTimeout(r, 300));
                if (message_box.current)
                {
                    message_box.current?.scrollTo({
                        top: message_box.current.scrollHeight,
                        behavior: "smooth"
                    });
                }
            }
            else if (message_box.current && (message_box.current.scrollTop || 0 <= message_box.current.scrollHeight || 0 / 2))
            {
                await new Promise((r) => setTimeout(r, 300));
                message_box.current?.scrollTo({
                    top: message_box.current.scrollHeight,
                    behavior: "smooth"
                });
            }
        });
    }, []);

    useEffect(() => {
        if (!messageList[0])
        {
            return;
        }

        if (!shouldLoadMore)
        {
            return;
        }
        Query<PaginatedType<MessageType>>(`/message?chat_id=${chat_id}&created_date=${messageList[messageList.length - 1].CreatedDate}&page=1&limit=30`).then((res) => {
            if (!res?.m_data)
            {
                setIsAtTop(true);
                return;
            }
            setMessageList([...messageList, ...res.m_data]);
            setShouldLoadMore(false);
        });
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

    const scrollCallback = React.useCallback(() => {
        if (!message_box.current)
        {
            return;
        }

        if ( message_box.current.scrollHeight - Math.abs(message_box.current.scrollTop) < message_box.current.scrollHeight / 5)
        {
           setShouldLoadMore(true);
           console.log("load more throttled 100ms")
        }
    }, []);

    function throttle (callbackFn: { (): void; (): void; call?: any; }, limit: number | undefined) {
        let wait = false;
        return function () {
            if (!wait) {
                callbackFn.call();
                wait = true;
                setTimeout(function () {
                    wait = false;
                }, limit);
            }
        }
    }

    useEffect(() => {

        if (!message_box.current)
        {
            return;
        }

        if (isAtTop)
        {
            message_box.current.removeEventListener("scroll", throttle(scrollCallback, 150));
            return;
        }
        message_box.current.addEventListener("scroll", throttle(scrollCallback, 150));

    }, [isAtTop]);

    return (
        <div className="w-full overflow-auto px-6 flex flex-col-reverse " ref={message_box} id="message_box">
            {

                messageList?.map((message, index) => {
                    const currentMessageDate = new Date(message.CreatedDate);
                    const nextMessageDate = new Date(messageList[index - 1]?.CreatedDate);
                    const offset = nextMessageDate.getTime() - currentMessageDate.getTime();

                    return (
                        <div key={message.Id + "_message_key"} id={message.Id}>
                            <MemoizedMessage message={message} setQuoteMessage={setQuoteMessage}/>
                            {
                                offset > 1000 * 60 * 30 ?
                                    <div className="text-center text-gray-500 text-sm"> {new Date(message.CreatedDate).toLocaleString("en-US", {
                                        month: "short",
                                        day: "numeric",
                                        year: "numeric",
                                        hour: "numeric",
                                        minute: "numeric",
                                        hour12: false
                                    })} </div>
                                    : null
                            }
                        </div>// <MessageMemo message={message} offset={offset} key={message.Id} />
                    );
                })
            }
        </div>
    );
}

export default MessageBox;

