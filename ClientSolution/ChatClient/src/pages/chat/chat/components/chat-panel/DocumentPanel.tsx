import React, { useEffect } from "react";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { codeFileExt, textFileExt } from "@/utils/file/fileExt.ts";
import textFile from "@/assets/text/text.png";
import { Tooltip } from "antd";
import FileOptions from "@/pages/chat/chat/components/chat-panel/FileOptions.tsx";
import { BsThreeDots } from "react-icons/bs";
import ViewMore from "@/components/button/ViewMore.tsx";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import MessageType from "@/utils/type/MessageType.ts";

function DocumentPanel()
{

    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [document, setDocument] = React.useState<MessageAttachType[]>([]);
    const [documentPage, setDocumentPage] = React.useState<number>(1);
    const documentContainer = React.useRef<HTMLDivElement>(null);

    useEffect(() => {
        documentContainer.current?.scrollTo({
            top: documentContainer.current.scrollHeight,
            behavior: "smooth"
        });
    }, [document]);

    useEffect(() => {
        Query<PaginatedType<MessageAttachType>>(`/message-attach?page=1&limit=5&chat_id=${chat_id}&type=text`)
            .then((data) => {
                if (data.m_data.length === 0)
                {
                    return;
                }
                setDocument(data.m_data);

                if(data.m_data.length < 5)
                {
                    setDocumentPage(-1);
                }
                else{
                    setDocumentPage((prev) => prev + 1);
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
                    if(attach.AttachType === "text")
                    {
                        setDocument((prev) => [attach, ...prev]);
                    }
                });
            }
        })
    }, []);

    return(
        <>
        <div className="max-h-96 overflow-auto" ref={documentContainer}>
            {
                document.map((d, index) => {
                    const ext = d.AttachName.split(".")[d.AttachName.split(".").length - 1];
                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === document.length - 1 ? "" : "border-b-2"}`}>
                            <div className="flex items-center">
                                {
                                    codeFileExt.includes(ext) || textFileExt.includes(ext) ?
                                        <img src={`/src/assets/text/${ext}.png`} alt="file" className="w-12 h-12 "/>
                                        : <img src={textFile} alt="file" className="w-12 h-12 "/>
                                }
                                <div className="ml-3">
                                    <h1 className="">{d.AttachName}</h1>
                                </div>
                            </div>
                            <Tooltip title={<FileOptions file={d.AttachUrl}/>} trigger={"click"} color={"white"}>
                                <button>
                                    <BsThreeDots className="text-2xl opacity-50"/>
                                </button>
                            </Tooltip>
                        </div>
                    );
                })
            }
        </div>
        <div className={`${documentPage === -1 ? "hidden" : ""}`}>
            <ViewMore func={ () => {
                if(documentPage === -1)
                {
                    return;
                }
                Query<PaginatedType<MessageAttachType>>(`/message-attach?page=${documentPage}&limit=5&chat_id=${chat_id}&type=text`)
                    .then((data) => {
                        if (!data)
                        {
                            setDocumentPage(-1);
                            return;
                        }
                        if(data.m_data.length < 5)
                        {
                            setDocumentPage(-1);
                        }
                        else
                        {
                            setDocumentPage((prev) => prev + 1);
                        }
                        setDocument(prevState => [...prevState, ...data.m_data]);
                    });
            } }/>
        </div>
    </>
    );
}

export default DocumentPanel;
