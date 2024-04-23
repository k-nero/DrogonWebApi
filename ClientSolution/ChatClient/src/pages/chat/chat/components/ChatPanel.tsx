import { TfiClose } from "react-icons/tfi";
import { Collapse, CollapseProps, Switch } from "antd";
import { BsThreeDots } from "react-icons/bs";
import React, { useEffect } from "react";
import Query from "@/utils/function/Query.ts";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { useLocation } from "react-router-dom";
import ImageViewModal from "@/components/modal/ImageViewModal.tsx";
import oogAudio from "@/assets/audio/oog-audio.png";
import webmAudio from "@/assets/audio/webm-audio.png";
import mp3Audio from "@/assets/audio/mp3-audio.png";
import wavAudio from "@/assets/audio/wav-audio.png";
import audioPng from "@/assets/audio/audio.png";
import AudioModal from "@/components/modal/AudioModal.tsx";


function ChatPanel({ setIsPanel }: { setIsPanel: () => void })
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [image, setImage] = React.useState<MessageAttachType[]>([]);
    const [file, setFile] = React.useState<MessageAttachType[]>([]);
    const [audio, setAudio] = React.useState<MessageAttachType[]>([]);
    const [video, setVideo] = React.useState<MessageAttachType[]>([]);
    const [document, setDocument] = React.useState<MessageAttachType[]>([]);

    useEffect(() => {
        Query<PaginatedType<MessageAttachType>> (`/message-attach?page=1&limit=20&chat_id=${chat_id}&type=image`)
            .then((data) => {
                setImage(data.m_data.map((image) => image));
            });
        Query<PaginatedType<MessageAttachType>> (`/message-attach?page=1&limit=20&chat_id=${chat_id}&type=application`)
            .then((data) => {
                setFile(data.m_data.map((file) => file));
            });
        Query<PaginatedType<MessageAttachType>> (`/message-attach?page=1&limit=20&chat_id=${chat_id}&type=audio`)
            .then((data) => {
                setAudio(data.m_data.map((audio) => audio));
            });
        Query<PaginatedType<MessageAttachType>> (`/message-attach?page=1&limit=20&chat_id=${chat_id}&type=video`)
            .then((data) => {
                setVideo(data.m_data.map((video) => video));
            });
        Query<PaginatedType<MessageAttachType>> (`/message-attach?page=1&limit=20&chat_id=${chat_id}&type=text`)
            .then((data) => {
                setDocument(data.m_data.map((document) => document));
            });
    }, []);

    const images = (
        <div className="grid-cols-3 grid gap-y-4 overflow-auto max-h-96">
            {
                image.map((img, index) => {
                    return (
                        <button onClick={() => {
                            ImageViewModal({ image: img});
                        }}>
                            <img key={index} loading="lazy" src={img.AttachUrl} alt={img.AttachName} className="h-32 m-auto"/>
                        </button>
                    );
                })
            }
        </div>
    );

    const audioList = (
        <div className="max-h-96 overflow-auto">
        {
                audio.map((a, index) => {
                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === audio.length - 1 ? "" : "border-b-2" }`}>
                            <button onClick={ () => {
                                AudioModal({audio: a});
                            }}>
                                <div className="flex items-center">
                                    {
                                        a.AttachName.split(".")[1] === "ogg" ?
                                            <img src={oogAudio} alt="audio" className="w-12 h-12 rounded-full"/>
                                            : a.AttachName.split(".")[1] === "webm" ?
                                                <img src={webmAudio} alt="audio" className="w-12 h-12 rounded-full"/>
                                                : a.AttachName.split(".")[1] === "mp3" ?
                                                    <img src={mp3Audio} alt="audio" className="w-12 h-12 rounded-full"/>
                                                    : a.AttachName.split(".")[1] === "wav" ?
                                                        <img src={wavAudio} alt="audio" className="w-12 h-12 rounded-full"/>
                                                        :
                                                        <img src={audioPng} alt="audio" className="w-12 h-12 rounded-full"/>

                                    }
                                    <div className="ml-3">

                                        <h1 className="">{a.AttachName}</h1>
                                        {
                                            // a.AttachName.split(".")[1] === "mp3" ?
                                            //     <audio controls>
                                            //         <source src={a.AttachUrl} type="audio/mpeg"/>
                                            //     </audio>
                                            //     : a.AttachName.split(".")[1] === "wav" ?
                                            //     <audio controls>
                                            //         <source src={a.AttachUrl} type="audio/wav"/>
                                            //     </audio>
                                            //     : a.AttachName.split(".")[1] === "ogg" ?
                                            //         <audio controls>
                                            //             <source src={a.AttachUrl} type="audio/ogg"/>
                                            //         </audio>
                                            //     : a.AttachName.split(".")[1] === "flac" ?
                                            //         <audio controls>
                                            //             <source src={a.AttachUrl} type="audio/flac"/>
                                            //         </audio>
                                            //     : a.AttachName.split(".")[1] === "aac" ?
                                            //         <audio controls>
                                            //             <source src={a.AttachUrl} type="audio/aac"/>
                                            //         </audio>
                                            //     : a.AttachName.split(".")[1] === "webm" ?
                                            //         <audio controls>
                                            //             <source src={a.AttachUrl} type="audio/webm"/>
                                            //         </audio> : null

                                        }
                                    </div>
                                </div>
                            </button>

                            <BsThreeDots className="text-2xl opacity-50"/>
                        </div>
                    );
                })
        }
        </div>
    );

    const videoList = (
        <div>
            {
                video.map((v, index) => {
                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === video.length - 1 ? "" : "border-b-2"}`}>
                            <div className="flex items-center">
                                <div className="ml-3">
                                    <h1 className="font-bold">{v.AttachName}</h1>
                                </div>
                            </div>
                            <BsThreeDots className="text-2xl opacity-50"/>
                        </div>
                    );
                })
            }
        </div>
    );

    const documentList = (
        <div>
            {
                document.map((d, index) => {
                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === document.length - 1 ? "" : "border-b-2"}`}>
                            <div className="flex items-center">
                                <div className="ml-3">
                                    <h1 className="font-bold">{d.AttachName}</h1>
                                </div>
                            </div>
                            <BsThreeDots className="text-2xl opacity-50"/>
                        </div>
                    );
                })
            }
        </div>
    );

    const fileList = (
        <div>
            {
                file.map((f, index) => {
                    return (
                        <div key={index} className={`flex items-center justify-between p-3 ${index === file.length - 1 ? "" : "border-b-2"}`}>
                            <div className="flex items-center">
                                <div className="ml-3">
                                    <h1 className="font-bold">{f.AttachName}</h1>
                                </div>
                            </div>
                            <BsThreeDots className="text-2xl opacity-50"/>
                        </div>
                    );
                })
            }
        </div>
    );

    const items: CollapseProps["items"] = [
        {
            key: "image",
            label: <p className="font-bold text-[15px]">Recent Images</p>,
            children: images
        },
        {
            key: "audio",
            label: <p className="font-bold text-[15px]">Recent Audio</p>,
            children: audioList
        },
        {
            key: "video",
            label: <p className="font-bold text-[15px]">Recent Videos</p>,
            children: videoList
        },
        {
            key: "document",
            label: <p className="font-bold text-[15px]">Recent Documents</p>,
            children: documentList
        },
        {
            key: "file",
            label: <p className="font-bold text-[15px]">Recent Files</p>,
            children: fileList,
        },
    ];



    return (
        <div className="p-6 border-l-2 max-h-screen overflow-auto">
            <div className="text-end">
                <button className="p-3 rounded-full border-2 w-fit" onClick={setIsPanel}>
                    <TfiClose  className=" text-1xl "/>
                </button>
            </div>
            <div className="mt-8 text-center border-b-2 pb-8">
                <img loading="lazy" src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 rounded-full m-auto border-4 border-teal-500"/>
                <h1 className="mt-4 font-bold text-2xl">John Doe</h1>
                <p className="text-sm text-gray-500">@john_doe</p>
            </div>
            <div className="flex justify-between py-6 px-4 border-b-2">
                <h1 className="font-bold">Notifications</h1>
                <Switch className="text-teal-500" />
            </div>
            <Collapse defaultActiveKey={[
                "image",
                "audio",
                "video",
                "document",
                "file",
            ]} expandIconPosition={"end"} items={items} className="text-teal-500 bg-white text-1xl border-b-2" bordered={false} />
        </div>
    );
}

export default ChatPanel;
