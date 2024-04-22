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
const cdnURL = new URL(`${import.meta.env.VITE_CDN_URL}`);

function MessageInput({messageList, quoteMessage, setQuoteMessages}:
                          {
                              messageList: MessageType[],
                              quoteMessage: MessageType | undefined,
                              setQuoteMessages: Dispatch<SetStateAction<MessageType | undefined>>,
                          })
{
    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;
    const textMessage = createRef<HTMLTextAreaElement>();
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [typing, setTyping] = useState<boolean>(false);
    const [timer, setTimer] = useState<NodeJS.Timeout>();
    const [message, setMessage] = useState<string>("");
    const [files, setFiles] = useState<File[]>([]);
    const [isSending, setIsSending] = useState<boolean>(false);

    useEffect(() => {
        uWebSockets.getInstance().addTypingSubscriber((event) => {
            const e_data = JSON.parse(event.data);
            if (e_data.channel === chat_id)
            {
                setTyping(e_data.typing);
            }
        });
    }, [chat_id]);

    useEffect(() => {
        if(files && files.length > 0)
        {

        }
    }, [files]);


    async function sendMessage()
    {
        if (uWebSockets.getInstance().ConnectState() !== 1)
        {
            alert("Connection lost, reconnecting...");
        }
        setQuoteMessages(undefined);

        setIsSending(true);
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

            if (files.length > 0)
            {
                Promise.all(files.map(async (file) => {
                    const formData = new FormData();
                    formData.append("file", file);
                    const fileRs = await fetch(`${cdnURL}files/uploads`, {
                        method: "PUT",
                        body: formData
                    });

                    if (fileRs.ok)
                    {
                        const r = await fetch(`${baseUrl}/message-attach`, {
                            method: "POST",
                            headers: {
                                "Content-Type": "application/json"
                            },
                            body: JSON.stringify({
                                MessageId: rs.id,
                                AttachUrl: `${cdnURL}files/${file.name}`,
                                AttachName: file.name,
                                AttachType: file.type.split("/")[0]
                            })
                        })
                    }
                })
                ).then(async () => {
                    const message = await Query<MessageType>(`/message/${rs.id}`);
                    uWebSockets.getInstance().send(JSON.stringify({
                        type: "message",
                        channel: chat_id,
                        message: message
                    }));
                });
            }
        }
        setFiles([]);
        setIsSending(false);
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
            <div className="px-8 py-4 rounded-xl mx-8 mt-4 bg-white">
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
                    ) : null
                }
                {
                    files.length > 0 ? (
                        <div className="flex gap-2">
                            {
                                files.map((file, index) => {
                                    return (
                                        <div key={index} className="">
                                            <div className="flex gap-2">
                                                <p className="max-w-32 overflow-clip">{file.name}</p>
                                                <button onClick={() => {
                                                    setFiles(files.filter((f) => f.name !== file.name));
                                                }}>

                                                    <IoClose/>
                                                </button>
                                            </div>
                                            {
                                                file.type.split("/")[0] === "image" ? (
                                                    <img src={URL.createObjectURL(file)} alt={file.name} className="w-32 h-32"/>
                                                ) : null
                                            }
                                        </div>
                                    );
                                })
                            }
                        </div>
                    ) : null
                }
                <form onSubmit={(e) => {
                    e.preventDefault();
                    e.currentTarget.reset();
                    sendMessage().then(() => {});
                }}

                className="mt-4"
                >
                    <div>
                <textarea autoFocus={true} placeholder="Type a message" ref={textMessage} onChange={
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
                    onFocus={(e) => handleFocus(e)}
                    onPaste={(e) => {
                        const fileList = e.clipboardData.files;
                        const filesArray = Array.from(fileList);
                        setFiles(filesArray);
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
                                <button onClick={(e) => {
                                    // open file dialog
                                    e.preventDefault();
                                    const fileInput = document.createElement("input");
                                    fileInput.type = "file";
                                    fileInput.multiple = true;
                                    fileInput.click();
                                    fileInput.addEventListener("change", (e) => {
                                        const fileList = fileInput.files;
                                        if(!fileList) return;
                                        const filesArray = Array.from(fileList);
                                        setFiles(filesArray);
                                    });

                                }}>
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
