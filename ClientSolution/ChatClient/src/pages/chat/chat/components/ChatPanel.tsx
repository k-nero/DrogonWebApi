import { TfiClose } from "react-icons/tfi";
import { Collapse, CollapseProps, Switch, Tooltip } from "antd";
import { BsThreeDots } from "react-icons/bs";
import React, { useEffect } from "react";
import Query from "@/utils/function/Query.ts";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";
import { useLocation } from "react-router-dom";
import ImageViewModal from "@/components/modal/ImageViewModal.tsx";
import audioPng from "@/assets/audio/audio.png";
import videoFile from "@/assets/video/video-file.png";
import textFile from "@/assets/text/text.png";
import filePng from "@/assets/file/file.png";
import AudioModal from "@/components/modal/AudioModal.tsx";
import{ codeFileExt, textFileExt, videoFileExt, audioFileExt, otherFileExt } from "@/utils/file/fileExt";

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

    function FileOptions()
    {
        return (
            <>
                <div className="min-w-16" style={{ color: "black" }}>
                    <div className="rounded-sm hover:bg-gray-300 p-1">
                        <button onClick={() => {
                        }}>
                            <p>Download</p>
                        </button>
                    </div>
                    <div className="rounded-sm hover:bg-gray-300 p-1">
                        <button onClick={() => {
                        }}>
                            <p>Copy</p>
                        </button>
                    </div>

                    <div className="rounded-sm hover:bg-gray-300 p-1">
                        <button>
                            <p>Share</p>
                        </button>
                    </div>
                </div>
            </>
        );
    }

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
                    const ext = a.AttachName.split(".")[a.AttachName.split(".").length - 1];

                    return (
                    <div key={index} className={`flex items-center justify-between p-3 ${index === audio.length - 1 ? "" : "border-b-2" }`}>
                            <button onClick={ () => {
                                AudioModal({audio: a});
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
    );

    const videoList = (
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
    );

    const documentList = (
        <div>
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
    );

    const fileList = (
        <div>
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
