import { useLocation } from "react-router-dom";
import React, { useEffect } from "react";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import AudioModal from "@/components/modal/AudioModal.tsx";
import { audioFileExt } from "@/utils/file/fileExt.ts";
import audioPng from "@/assets/audio/audio.png";
import { Tooltip } from "antd";
import { BsThreeDots } from "react-icons/bs";
import ViewMore from "@/components/button/ViewMore.tsx";
import FileOptions from "@/pages/chat/chat/components/chat-panel/FileOptions.tsx";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import MessageType from "@/utils/type/MessageType.ts";

function AudioPanel()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [audio, setAudio] = React.useState<MessageAttachType[]>([]);
    const [audioPage, setAudioPage] = React.useState<number>(1);

    useEffect(() => {
        Query<PaginatedType<MessageAttachType>>(`/message-attach?page=1&limit=5&chat_id=${chat_id}&type=audio`)
            .then((data) => {
                if (data.m_data.length === 0)
                {
                    return;
                }
                if(data.m_data.length < 5)
                {
                    setAudioPage(-1);
                }
                else
                {
                    setAudioPage((prev) => prev + 1);
                }
                setAudio(data.m_data);
            });
    }, [chat_id]);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSubscriber((event) => {
            const soc_mess: SocketMessageType = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;
            if (message.ChatRoomId !== chat_id)
            {
                return;
            }

            if(message.MessageAttachs)
            {
                message.MessageAttachs.forEach((attach) => {
                    if(attach.AttachType === "audio")
                    {
                        setAudio((prev) => [attach, ...prev]);
                    }
                });
            }
        })
    }, []);

    return(
        <>
            <div className="max-h-96 overflow-auto">
                {
                    audio.map((a, index) => {
                        const ext = a.AttachName.split(".")[a.AttachName.split(".").length - 1];

                        return (
                            <div key={index} className={`flex items-center justify-between p-3 ${index === audio.length - 1 ? "" : "border-b-2"}`}>
                                <button onClick={() => {
                                    AudioModal({ audio: a });
                                }}>
                                    <div className="flex items-center">
                                        {
                                            audioFileExt.includes(ext) ?
                                                <img src={`/src/assets/audio/${ext}.png`} alt="file" className="w-12 h-12 "/>
                                                : <img src={audioPng} alt="file" className="w-12 h-12 "/>

                                        }
                                        <div className="ml-3">
                                            <h1 className="">{a.AttachName}</h1>
                                        </div>
                                    </div>
                                </button>

                                <Tooltip title={<FileOptions/>} trigger={"click"} color={"white"}>
                                    <button>
                                        <BsThreeDots className="text-2xl opacity-50"/>
                                    </button>
                                </Tooltip>
                            </div>
                        );
                    })
                }
            </div>
            <div className={`${audioPage === -1 ? "hidden" : ""}`}>
                <ViewMore func={() => {
                    if(audioPage === -1)
                    {
                        return;
                    }
                    Query<PaginatedType<MessageAttachType>>(`/message-attach?page=${audioPage}&limit=5&chat_id=${chat_id}&type=audio`)
                        .then((data) => {
                            if (!data)
                            {
                                setAudioPage(-1);
                                return;
                            }
                            if(data.m_data.length < 5)
                            {
                                setAudioPage(-1);
                            }
                            else
                            {
                                setAudioPage((prev) => prev + 1);
                            }
                            setAudio(prevState => [...prevState, ...data.m_data]);
                        });
                }}/>
            </div>
        </>
    );
}

export default AudioPanel;
