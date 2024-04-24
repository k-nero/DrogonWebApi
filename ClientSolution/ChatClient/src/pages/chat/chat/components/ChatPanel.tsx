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
import { audioFileExt, codeFileExt, otherFileExt, textFileExt, videoFileExt } from "@/utils/file/fileExt";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessageType from "@/utils/WebSocket/SocketMessageType.ts";
import MessageType from "@/utils/type/MessageType.ts";

function ChatPanel({ setIsPanel }: { setIsPanel: () => void })
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [image, setImage] = React.useState<MessageAttachType[]>([]);
    const [imagePage, setImagePage] = React.useState<number>(1);
    const [file, setFile] = React.useState<MessageAttachType[]>([]);
    const [filePage, setFilePage] = React.useState<number>(1);
    const [audio, setAudio] = React.useState<MessageAttachType[]>([]);
    const [audioPage, setAudioPage] = React.useState<number>(1);
    const [video, setVideo] = React.useState<MessageAttachType[]>([]);
    const [videoPage, setVideoPage] = React.useState<number>(1);
    const [document, setDocument] = React.useState<MessageAttachType[]>([]);
    const [documentPage, setDocumentPage] = React.useState<number>(1);

    function Init()
    {
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
    }

    useEffect(() => {
       Init();
    }, []);


    useEffect(() => {
        uWebSockets.getInstance().addMessageSubscriber((event) => {
            const soc_mess: SocketMessageType = JSON.parse(event.data);
            const message: MessageType = soc_mess.message;
            if (message.ChatRoomId !== chat_id)
            {
                return;
            }
            if (message.MessageAttachs?.length !== 0 )
            {
                Init();
            }

        })
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

    function ViewMore({ func }: { func?: () => void })
    {
        return (
            <div className="w-full mt-4 mx-4 text-center">
                <button className="bg-teal-500 w-96 h-7 rounded-xl" onClick={func}>
                    <p className="text-center text-white font-bold">View More</p>
                </button>
            </div>
        );

    }

    const images = (
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

    const audioList = (
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

    const videoList = (
        <>
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

    const documentList = (
        <>
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

    const fileList = (
        <>
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
            children: fileList
        }
    ];


    return (
        <div className="p-6 border-l-2 max-h-screen overflow-auto">
            <div className=" text-end ">
                {/*<button className="p-3 fixed bg-white right-8 border-teal-500 rounded-full border-2 w-fit" onClick={setIsPanel}>*/}
                    <button className="p-3 bg-white  border-teal-500 rounded-full border-2 w-fit" onClick={setIsPanel}>
                        <TfiClose className=" text-1xl "/>
                    </button>
            </div>
            <div className="mt-8 text-center border-b-2 pb-8">
                <img loading="lazy" src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 rounded-full m-auto border-4 border-teal-500"/>
                <h1 className="mt-4 font-bold text-2xl">John Doe</h1>
                <p className="text-sm text-gray-500">@john_doe</p>
            </div>
            <div className="flex justify-between py-6 px-4 border-b-2">
                <h1 className="font-bold">Notifications</h1>
                <Switch className="text-teal-500"/>
            </div>
            <Collapse defaultActiveKey={[
                "image",
                "audio",
                "video",
                "document",
                "file"
            ]} expandIconPosition={"end"} items={items} className="text-teal-500 bg-white text-1xl border-b-2" bordered={false}/>
        </div>
    );
}

export default ChatPanel;
