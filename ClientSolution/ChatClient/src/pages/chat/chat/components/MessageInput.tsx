import { GoDeviceCameraVideo } from "react-icons/go";
import { GrAttachment } from "react-icons/gr";
import { IoClose, IoMicOutline, IoSendSharp } from "react-icons/io5";
import { LuLoader2, LuSmile } from "react-icons/lu";
import { HiDotsVertical } from "react-icons/hi";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import React, { ChangeEvent, createRef, Dispatch, SetStateAction, useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import MessageType from "@/utils/type/MessageType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";
import videoPlaceholder from "@/assets/video-placeholder.png";
import textPlaceholder from "@/assets/text-placeholder.png";
import audioPlaceholder from "@/assets/audio-placeholder.png";
import filePlaceholder from "@/assets/file-placeholder.png";
import { Modal, Tooltip } from "antd";
import { AudioRecorder } from "react-audio-voice-recorder";
import EmojiTooltip from "@/components/EmojiToolTip.tsx";
import { EmojiClickData } from "emoji-picker-react";

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);
const cdnURL = new URL(`${import.meta.env.VITE_CDN_URL}`);

function MessageInput({ messageList, quoteMessage, setQuoteMessages }:
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
        if (files && files.length > 0)
        {

        }
    }, [files]);

    const [isVoiceModalOpen, setVoiceModalOpen] = useState(false);
    const showVoiceModal = () => {
        setVoiceModalOpen(true);
    };
    const handleVoiceOk = () => {
        setVoiceModalOpen(false);
    };
    const handleVoiceCancel = () => {
        setVoiceModalOpen(false);
    };

    function handleVoiceRecordingComplete(blob: Blob)
    {
        const file = new File([blob], "audio.webm", { type: "audio/webm" });
        setFiles([...files, file]);
    }

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
                            });
                        }
                    })
                ).then(async () => {
                    setFiles([]);
                    const message = await Query<MessageType>(`/message/${rs.id}`);
                    uWebSockets.getInstance().send(JSON.stringify({
                        type: "message",
                        channel: chat_id,
                        message: message
                    }));
                });
            }
            else
            {
                const message = await Query<MessageType>(`/message/${rs.id}`);
                uWebSockets.getInstance().send(JSON.stringify({
                    type: "message",
                    channel: chat_id,
                    message: message
                }));
            }
        }
        setIsSending(false);
    }

    function handleFocus(e: ChangeEvent<HTMLTextAreaElement>)
    {
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

    function handleEmojiClickCallback(emoji: EmojiClickData, event: MouseEvent)
    {
        const m = message + emoji.emoji;
        setMessage(m);
    }

    return (
        <div>
            <Modal title={"Voice message"} open={isVoiceModalOpen} centered={true} onOk={handleVoiceOk} onCancel={handleVoiceCancel} footer={null} width="30%">
                <div className="text-center">
                    <p className="">Press the button to start recording</p>
                    <div className="m-auto w-fit h-fit mt-4">
                        <AudioRecorder onRecordingComplete={handleVoiceRecordingComplete}
                                       audioTrackConstraints={{
                                           noiseSuppression: true,
                                           echoCancellation: true,
                                           autoGainControl: true
                                       }}
                                       onNotAllowedOrFound={(err) => console.table(err)}
                                       downloadOnSavePress={false}
                                       downloadFileExtension="webm"
                                       mediaRecorderOptions={{
                                           audioBitsPerSecond: 128000
                                       }}
                                       showVisualizer={true}
                        />
                    </div>
                </div>
            </Modal>
            <div className={`flex items-center justify-center text-teal-500 ${!typing ? "hidden" : ""}`}>
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
                                        <IoClose/>
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
                                                <p className="w-32 overflow-hidden whitespace-nowrap">{file.name}</p>
                                                <button onClick={() => {
                                                    setFiles(files.filter((f) => f.name !== file.name));
                                                }}>

                                                    <IoClose/>
                                                </button>
                                            </div>
                                            {

                                                file.type.split("/")[0] === "image" ? (
                                                        <img src={URL.createObjectURL(file)} alt={file.name} className="max-h-32 max-w-32 m-auto"/>
                                                    ) :
                                                    file.type.split("/")[0] === "video" ? (
                                                            <div>
                                                                <img src={videoPlaceholder} className="max-h-32 max-w-32 m-auto" alt="video placeholder"/>
                                                            </div>)
                                                        : file.type.split("/")[0] === "text" ? (
                                                                <div>
                                                                    <img src={textPlaceholder} className="max-h-32 max-w-32 m-auto" alt="text placeholder"/>
                                                                </div>)
                                                            :
                                                            file.type.split("/")[0] === "audio" ? (
                                                                    <div>
                                                                        <img src={audioPlaceholder} className="max-h-32 max-w-32 m-auto" alt="text placeholder"/>
                                                                    </div>)
                                                                :
                                                                (<div>
                                                                    <img src={filePlaceholder} className="max-h-32 max-w-32 m-auto" alt="file placeholder"/>
                                                                </div>)
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
                <textarea autoFocus={true} placeholder="Type a message" ref={textMessage} value={message} onChange={
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
                              setFiles(prevState => {
                                    return [...prevState, ...filesArray];
                              });
                          }}
                          className="w-full bg-transparent outline-none pb-4 border-b-2 resize-none"/>
                    </div>
                    <div className="flex justify-between text-2xl mt-4">
                        <div className="flex gap-3">
                            <div className="flex gap-3 pr-3 border-r-2">
                                <button onClick={(e) => {
                                    e.preventDefault();
                                }}>
                                    <GoDeviceCameraVideo/>
                                </button>
                                <button onClick={(e) => {
                                    e.preventDefault();
                                    showVoiceModal();
                                }}>
                                    <IoMicOutline/>
                                </button>
                            </div>
                            <div className="flex gap-3 pr-3 border-r-2">
                                <Tooltip title={
                                    <EmojiTooltip reactionOpen={false} EmojiClickCallback={handleEmojiClickCallback}
                                    />} trigger={"click"} color={"white"} overlayInnerStyle={{
                                    padding: "0px",
                                    borderRadius: "32px"
                                }} overlayStyle={{ maxWidth: "500px" }}>
                                    <button onClick={(e) => {
                                        e.preventDefault();
                                    }}
                                            title={"Emoji"}
                                    >
                                        <LuSmile/>
                                    </button>
                                </Tooltip>
                                <button onClick={(e) => {
                                    // open file dialog
                                    e.preventDefault();
                                    const fileInput = document.createElement("input");
                                    fileInput.type = "file";
                                    fileInput.multiple = true;
                                    fileInput.click();
                                    fileInput.addEventListener("change", (e) => {
                                        const fileList = fileInput.files;
                                        if (!fileList)
                                        {
                                            return;
                                        }
                                        const filesArray = Array.from(fileList);
                                        setFiles(prevState => {
                                            return [...prevState, ...filesArray];
                                        });
                                    });

                                }}>
                                    <GrAttachment/>
                                </button>
                            </div>
                            <div>
                                <button className="mt-2" onClick={(e) => {
                                    e.preventDefault();
                                }}>
                                    <HiDotsVertical/>
                                </button>
                            </div>
                        </div>

                        <div className="justify-items-end">
                            <button className="text-teal-500 font-bold" type="submit" onClick={(e) => {
                                e.preventDefault();
                            }}>
                                {
                                    isSending ? (
                                        <div className="animate-spin">
                                            <LuLoader2/>
                                        </div>
                                    ) : (
                                        <IoSendSharp/>
                                    )
                                }
                            </button>
                        </div>
                    </div>
                </form>
            </div>
        </div>

    );
}

export default MessageInput;
