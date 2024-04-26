import { Collapse, CollapseProps, Switch, Tooltip } from "antd";
import { BsThreeDots } from "react-icons/bs";
import React from "react";
import { useLocation } from "react-router-dom";
import ChatRoom from "@/utils/type/ChatRoom.ts";
import ImagePanel from "@/pages/chat/chat/components/chat-panel/ImagePanel.tsx";
import AudioPanel from "@/pages/chat/chat/components/chat-panel/AudioPanel.tsx";
import FileOptions from "@/pages/chat/chat/components/chat-panel/FileOptions.tsx";
import VideoPanel from "@/pages/chat/chat/components/chat-panel/VideoPanel.tsx";
import DocumentPanel from "@/pages/chat/chat/components/chat-panel/DocumentPanel.tsx";
import FilePanel from "@/pages/chat/chat/components/chat-panel/FilePanel.tsx";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import OnlineSignalDot from "@/components/OnlineSignalDot.tsx";

function ChatPanel({chatRoom, onlineMap} : {chatRoom : ChatRoom | undefined, onlineMap: Map<string, boolean>})
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;

    //
    // const [chatRoom, setChatRoom] = React.useState<ChatRoom>();
    //
    // function Init()
    // {
    //     Query<ChatRoom>(`/chat-room/${chat_id}`)
    //         .then((data) => {
    //             if (!data)
    //             {
    //                 return;
    //             }
    //
    //             setChatRoom(data);
    //         });
    // }
    //
    // useEffect(() => {
    //    Init();
    // }, []);
    //
    // useEffect(() => {
    //     uWebSockets.getInstance().addMessageSubscriber((event) => {
    //         const soc_mess: SocketMessageType = JSON.parse(event.data);
    //         const message: MessageType = soc_mess.message;
    //         if (message.ChatRoomId !== chat_id)
    //         {
    //             return;
    //         }
    //
    //     })
    // }, []);

    const chatMemberList = (
        <>
            <div className="overflow-auto max-h-96">
                {
                    chatRoom?.ChatParticipants?.map((member, index) => {
                        const len = chatRoom?.ChatParticipants?.length || 0;
                        return (
                            <div className={`${ index === len - 1 ? "mt-4" : "" } flex gap-2.5`} key={member.Id}>
                                <div className="relative">
                                    <img key={index} loading="lazy" src={member.ApplicationUser?.AvatarUrl} alt={member.ApplicationUser?.UserName} className="h-10 rounded-full" style={{
                                        objectFit: "contain"
                                    }}/>
                                    {
                                       credential?.user.Id === member.ApplicationUserId ? <OnlineSignalDot online={true}/> : <>
                                       {
                                            onlineMap.get(member.ApplicationUserId) ? <OnlineSignalDot online={true}/> : <OnlineSignalDot online={false}/>
                                       }
                                       </>
                                    }
                                </div>
                                <div>
                                <p className="">
                                        @<a href="#" className="underline">{member.ApplicationUser?.UserName}</a>
                                    </p>
                                    <p className="text-sm text-gray-600">
                                        Joined on
                                        {
                                            " " + new Date(member.CreatedDate).toLocaleDateString()
                                        }
                                    </p>
                                </div>
                                <div className="ml-auto flex items-center gap-4">
                                    <div>
                                        Member
                                    </div>
                                    <Tooltip  title={<FileOptions/>} trigger={"click"} color={"white"}>
                                        <button>
                                            <BsThreeDots className="text-2xl opacity-50"/>
                                        </button>
                                    </Tooltip>
                                </div>
                            </div>
                        );
                    })
                }

            </div>
        </>
    );

    const items: CollapseProps["items"] = [
        {
            key: "chatMember",
            label: <p className="font-bold text-[15px]">Members</p>,
            children: chatMemberList
        },
        {
            key: "image",
            label: <p className="font-bold text-[15px]">Recent Images</p>,
            children: <ImagePanel/>
        },
        {
            key: "audio",
            label: <p className="font-bold text-[15px]">Recent Audio</p>,
            children: <AudioPanel/>
        },
        {
            key: "video",
            label: <p className="font-bold text-[15px]">Recent Videos</p>,
            children: <VideoPanel/>
        },
        {
            key: "document",
            label: <p className="font-bold text-[15px]">Recent Documents</p>,
            children: <DocumentPanel/>
        },
        {
            key: "file",
            label: <p className="font-bold text-[15px]">Recent Files</p>,
            children: <FilePanel/>
        }
    ];

    return (
        <div className="p-6 border-l-2 max-h-screen overflow-auto">
            {/*<div className=" text-end ">*/}
            {/*    /!*<button className="p-3 fixed bg-white right-8 border-teal-500 rounded-full border-2 w-fit" onClick={setIsPanel}>*!/*/}
            {/*        <button className="p-3 bg-white  border-teal-500 rounded-full border-2 w-fit" onClick={setIsPanel}>*/}
            {/*            <TfiClose className=" text-1xl "/>*/}
            {/*        </button>*/}
            {/*</div>*/}
            <div className="mt-8 text-center pb-8">
                <img loading="lazy" src={chatRoom?.RoomImageUrl} alt="John Doe" className="w-32 h-32 rounded-full m-auto "/>
                {/*<h1 className="mt-4 font-bold text-2xl"></h1>*/}
                {/*<p className="text-sm text-gray-500">@john_doe</p>*/}
            </div>
            <div className="flex justify-between py-6 px-4 border-b-2">
                <h1 className="font-bold">Notifications</h1>
                <Switch className="text-teal-500"/>
            </div>
            <Collapse defaultActiveKey={[
                "chatMember",
                "image"
            ]} expandIconPosition={"end"} items={items} className="text-teal-500 bg-white text-1xl border-b-2" bordered={false}/>
        </div>
    );
}

export default ChatPanel;
