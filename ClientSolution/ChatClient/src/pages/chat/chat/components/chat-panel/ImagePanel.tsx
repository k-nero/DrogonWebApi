import React, { useEffect } from "react";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import { useLocation } from "react-router-dom";
import Query from "@/utils/function/Query.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import ImageViewModal from "@/components/modal/ImageViewModal.tsx";
import ViewMore from "@/components/button/ViewMore.tsx";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import MessageType from "@/utils/type/MessageType.ts";

function ImagePanel()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [image, setImage] = React.useState<MessageAttachType[]>([]);
    const [imagePage, setImagePage] = React.useState<number>(1);

    useEffect(() => {
        Query<PaginatedType<MessageAttachType>>(`/message-attach?page=1&limit=6&chat_id=${chat_id}&type=image`)
            .then((data) => {
                if (data.m_data.length === 0)
                {
                    return;
                }
                if(data.m_data.length < 6)
                {
                    setImagePage(-1);
                }
                else
                {
                    setImagePage((prev) => prev + 1);
                }
                setImage(data.m_data);
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
                    if(attach.AttachType === "image")
                    {
                        setImage((prev) => [attach, ...prev]);
                    }
                });
            }
        })
    }, []);

    return (
        <>
            <div className="grid-cols-3 grid gap-4 overflow-auto max-h-96">
                {
                    image.map((img, index) => {
                        return (
                            <button key={img.Id} onClick={() => {
                                ImageViewModal({ image: img });
                            }}>
                                <img key={index} loading="lazy" src={img.AttachUrl} alt={img.AttachName} className="h-32 m-auto" style={{
                                    objectFit: "contain"
                                }}/>
                            </button>
                        );
                    })
                }
            </div>
            <div className={`${imagePage === -1 ? "hidden" : ""}`}>
                <ViewMore func={() => {
                    if(imagePage === -1)
                    {
                        return;
                    }
                    Query<PaginatedType<MessageAttachType>>(`/message-attach?page=${imagePage}&limit=6&chat_id=${chat_id}&type=image`)
                        .then((data) => {
                            if (!data)
                            {
                                setImagePage(-1);
                                return;
                            }
                            if(data.m_data.length < 6)
                            {
                                setImagePage(-1);
                            }
                            else
                            {
                                setImagePage((prev) => prev + 1);
                            }
                            setImage(prevState => [...prevState, ...data.m_data]);
                        });
                }}/>
            </div>
            </>
    );
}

export default ImagePanel;
