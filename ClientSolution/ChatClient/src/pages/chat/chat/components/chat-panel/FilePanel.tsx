import { useLocation } from "react-router-dom";
import React, { useEffect } from "react";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { otherFileExt, textFileExt } from "@/utils/file/fileExt.ts";
import filePng from "@/assets/file/file.png";
import { Tooltip } from "antd";
import FileOptions from "@/pages/chat/chat/components/chat-panel/FileOptions.tsx";
import { BsThreeDots } from "react-icons/bs";
import ViewMore from "@/components/button/ViewMore.tsx";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import MessageType from "@/utils/type/MessageType.ts";

function FilePanel()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];


    const [file, setFile] = React.useState<MessageAttachType[]>([]);
    const [filePage, setFilePage] = React.useState<number>(1);
    const fileContainer = React.useRef<HTMLDivElement>(null);

    useEffect(() => {
        fileContainer.current?.scrollTo({
            top: fileContainer.current.scrollHeight,
            behavior: "smooth"
        });
    }, [file]);

    useEffect(() => {
        Query<PaginatedType<MessageAttachType>>(`/message-attach?page=1&limit=5&chat_id=${chat_id}&type=application`)
            .then((data) => {
                if (data.m_data.length === 0)
                {
                    return;
                }
                if(data.m_data.length < 5)
                {
                    setFilePage(-1);
                }
                else
                {
                    setFilePage((prev) => prev + 1);
                }
                setFile(data.m_data);
            });
    }, []);

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
                    if(attach.AttachType === "application")
                    {
                        setFile((prev) => [attach, ...prev]);
                    }
                });
            }
        })
    }, []);

    return (  <>
        <div className="max-h-96 overflow-auto"  ref={fileContainer}>
            {
                file.map((f, index) => {
                    const ext = f.AttachName.split(".")[f.AttachName.split(".").length - 1];
                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === file.length - 1 ? "" : "border-b-2"}`}>
                            <div className="flex items-center">
                                {
                                    otherFileExt.includes(ext) || textFileExt.includes(ext) ?
                                        <img src={`/src/assets/file/${ext}.png`} alt="file" className="w-12 h-12 "/>
                                        : <img src={filePng} alt="file" className="w-12 h-12 "/>
                                }
                                <div className="ml-3">
                                    <h1 className="">{f.AttachName}</h1>
                                </div>
                            </div>
                            <Tooltip title={<FileOptions file={f.AttachUrl}/>} trigger={"click"} color={"white"}>
                                <button>
                                    <BsThreeDots className="text-2xl opacity-50"/>
                                </button>
                            </Tooltip>
                        </div>
                    );
                })
            }
        </div>
        <div className={`${filePage === -1 ? "hidden" : ""}`}>
            <ViewMore func={() => {
                if (filePage === -1)
                {
                    return;
                }
                Query<PaginatedType<MessageAttachType>>(`/message-attach?page=${filePage}&limit=5&chat_id=${chat_id}&type=application`)
                    .then((data) => {
                        if (!data)
                        {
                            setFilePage(-1);
                            return;
                        }
                        if(data.m_data.length < 5)
                        {
                            setFilePage(-1);
                        }
                        else
                        {
                            setFilePage((prev) => prev + 1);
                        }
                        setFile(prevState => [...prevState, ...data.m_data]);
                    });
            }}/>
        </div>
    </>
    );
}

export default FilePanel;
