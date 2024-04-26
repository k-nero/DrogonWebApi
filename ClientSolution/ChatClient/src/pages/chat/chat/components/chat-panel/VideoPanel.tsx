import React, { useEffect } from "react";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { videoFileExt } from "@/utils/file/fileExt.ts";
import videoFile from "@/assets/video/video-file.png";
import { Tooltip } from "antd";
import FileOptions from "@/pages/chat/chat/components/chat-panel/FileOptions.tsx";
import { BsThreeDots } from "react-icons/bs";
import ViewMore from "@/components/button/ViewMore.tsx";
import MessageType from "@/utils/type/MessageType";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";

function VideoPanel()
{

    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [video, setVideo] = React.useState<MessageAttachType[]>([]);
    const [videoPage, setVideoPage] = React.useState<number>(1);

    useEffect(() => {
        Query<PaginatedType<MessageAttachType>>(`/message-attach?page=1&limit=5&chat_id=${chat_id}&type=video`)
            .then((data) => {
                if (data.m_data.length === 0)
                {
                    return;
                }
                setVideo(data.m_data);

                if(data.m_data.length < 5)
                {
                    setVideoPage(-1);
                }
                else
                {
                    setVideoPage((prev) => prev + 1);
                }
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
                    if(attach.AttachType === "video")
                    {
                        setVideo((prev) => [attach, ...prev]);
                    }
                });
            }
        })
    }, []);

    return (<>
        <div>
            {
                video.map((v, index) => {
                    const ext = v.AttachName.split(".")[v.AttachName.split(".").length - 1];

                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === video.length - 1 ? "" : "border-b-2"}`}>
                            <div className="flex items-center">
                                <div>
                                    {
                                        videoFileExt.includes(ext) ?
                                            <img src={`/src/assets/video/${ext}.png`} alt="file" className="w-12 h-12 "/>
                                            : <img src={videoFile} alt="file" className="w-12 h-12 "/>

                                    }
                                </div>
                                <div className="ml-3">
                                    <h1 className="">{v.AttachName}</h1>
                                </div>
                            </div>
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
        <div className={`${videoPage === -1 ? "hidden" : ""}`}>
            <ViewMore func={() => {
                if(videoPage === -1)
                {
                    return;
                }
                Query<PaginatedType<MessageAttachType>>(`/message-attach?page=${videoPage}&limit=5&chat_id=${chat_id}&type=video`)
                    .then((data) => {
                        if (!data)
                        {
                            setVideoPage(-1);
                            return;
                        }
                        if(data.m_data.length < 5)
                        {
                            setVideoPage(-1);
                        }
                        else
                        {
                            setVideoPage((prev) => prev + 1);
                        }
                        setVideo(prevState => [...prevState, ...data.m_data]);
                    });
            }}/>
        </div>
    </>
    );
}

export default VideoPanel;
