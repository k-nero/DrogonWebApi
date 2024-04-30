import { GoDotFill } from "react-icons/go";
import { BiSolidPhoneCall } from "react-icons/bi";
import React from "react";
import { BsCameraVideoFill, BsThreeDots } from "react-icons/bs";
import ChatRoom from "@/utils/type/ChatRoom.ts";
import { Skeleton } from "antd";

function ChatBoxHeader({setIsPanel, chat_room, onlineMap} : {setIsPanel: () => void, chat_room : ChatRoom | undefined, onlineMap: Map<string, boolean>})
{
    return (
        <div className="w-full bg-white">
            <div className="flex items-center justify-between p-6 border-b-2">
                <div className="flex items-center">
                    {
                        chat_room ?
                            <img src={chat_room?.RoomImageUrl} alt={chat_room?.RoomName} className="w-12 h-12 rounded-full"/>
                            : <Skeleton.Avatar active size="large"/>
                    }
                    <div className="ml-3">
                        {
                            chat_room ?
                                <h1 className="font-bold">{chat_room?.RoomName}</h1>
                                : <Skeleton.Input style={{ width: 200, height: "16px" }} active />
                        }
                        <div >
                            {
                                chat_room ?
                                Array.from(onlineMap.values()).filter(function(item){
                                    return item;
                                }).length === 0 ?
                                    (<div className="flex"><GoDotFill className="text-gray-500 text-2xl"/>
                                        <p className="text-sm text-gray-500 pt-[2px]">Offline</p></div>)
                                    :
                                    (<div className="flex"><GoDotFill className="text-green-600 text-2xl"/>
                                        <p className="text-sm text-gray-500 pt-[2px]">{Array.from(onlineMap.values()).filter(function(item){ return item;}).length > 1
                                            ? Array.from(onlineMap.values()).filter(function(item){return item;}).length
                                            : "" } Online</p></div>)
                                    : <Skeleton.Input style={{ width: 100, height: "16px"}} active />
                            }

                        </div>
                    </div>
                </div>
                <div className="flex gap-2.5">
                    <button className="p-3 rounded-full border-2">
                        <BiSolidPhoneCall className="text-teal-600 text-2xl"  />
                    </button>
                    <button className="p-3 rounded-full border-2">
                        <BsCameraVideoFill className="text-teal-600 text-2xl"/>
                    </button>
                    <button className="p-3 rounded-full border-2" onClick={setIsPanel}>
                        <BsThreeDots className="text-2xl text-teal-500 opacity-50"/>
                    </button>
                </div>
            </div>
        </div>
    );
}

export default ChatBoxHeader;
