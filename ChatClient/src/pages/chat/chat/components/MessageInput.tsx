import { GoDeviceCameraVideo } from "react-icons/go";
import { GrAttachment } from "react-icons/gr";
import { IoClose, IoMicOutline, IoSendSharp } from "react-icons/io5";
import { LuSmile } from "react-icons/lu";
import { HiDotsVertical } from "react-icons/hi";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { ChangeEvent, createRef, Dispatch, SetStateAction, useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import MessageType from "@/utils/type/MessageType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function MessageInput({messageList, setMessageList, quoteMessage, setQuoteMessages}:
                          {messageList: MessageType[],
                              setMessageList: Dispatch<SetStateAction<MessageType[]>>,
                              quoteMessage: MessageType | undefined,
                                setQuoteMessages: Dispatch<SetStateAction<MessageType | undefined>>})
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
        if (uWebSockets.getInstance().ConnectState() !== 1)
        {
            alert("Connection lost, reconnecting...");
        }
        setQuoteMessages(undefined);

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
                ApplicationUserId: user.user.Id,
                QuoteMessageId: quoteMessage?.Id ? quoteMessage.Id : undefined
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

    function handleFocus(e: ChangeEvent<HTMLTextAreaElement>)
    {
        messageList.reverse().map((message) => {
            if(message.ApplicationUserId !== user.user.Id)
            {
                if(message.MessageSeenBys?.findIndex((seenBy) => seenBy.ApplicationUserId === user.user.Id) === -1)
                {

                    fetch(`${baseUrl}/message-seen-by`, {
                        method: "POST",
                        headers: {
                            "Content-Type": "application/json",
                        },
                        body: JSON.stringify(
                            {
                                ApplicationUserId: user.user.Id,
                                MessageId: message.Id
                            }
                        )
                    }).then(r => {
                        if(r.ok)
                        {
                            r.json().then((rs) => {
                                Query<MessageSeenByType>(`/message-seen-by/${rs.id}`).then((r) => {

                                    setMessageList((prev) => {
                                        return prev.map((m) => {
                                            if(m.Id === message.Id)
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
                    })
                }
            }
        });
        messageList.reverse();
    }

    function handleTyping(e: ChangeEvent<HTMLTextAreaElement>)
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
                {
                    quoteMessage ? (
                        <div className="">
                            <div className="flex justify-between">
                                <div>
                                    <p className="font-bold">Replying to {
                                        quoteMessage?.ApplicationUser?.UserName === user.user.UserName ? "You" : "@" + quoteMessage?.ApplicationUser?.UserName
                                    }</p>
                                    <p>{quoteMessage.TextMessage}</p>
                                </div>
                                <div>
                                    <button onClick={() => {
                                        setQuoteMessages(undefined);
                                    }}>
                                        <IoClose />
                                    </button>
                                </div>
                            </div>
                        </div>
                    ) : (
                        <></>
                    )
                }
                <form onSubmit={(e) => {
                    e.preventDefault();
                    e.currentTarget.reset();
                    sendMessage().then(() => {});
                }}

                className="mt-4"
                >
                    <div>
                <textarea placeholder="Type a message" ref={textMessage} onChange={
                    (e) => {
                        setMessage(e.target.value);
                        handleTyping(e);
                    }
                } onKeyDown={(event) => {

                    if (event.key === "Enter" && !event.shiftKey)
                    {
                        event.preventDefault();
                        sendMessage().then(() => {

                        });
                        event.currentTarget.value = "";
                    }
                }}
                          onFocus={(e) => {
                                handleFocus(e);
                          }}

                          className="w-full bg-transparent outline-none pb-4 border-b-2 resize-none"/>
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
