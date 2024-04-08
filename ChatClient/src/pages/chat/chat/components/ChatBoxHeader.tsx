import { GoDotFill } from "react-icons/go";
import { BiSolidPhoneCall } from "react-icons/bi";
import React from "react";
import { BsCameraVideoFill, BsThreeDots } from "react-icons/bs";
import ChatRoom from "@/utils/type/ChatRoom.ts";

function ChatBoxHeader({setIsPanel, chat_room, onlineMap} : {setIsPanel: () => void, chat_room : ChatRoom | undefined, onlineMap: Map<string, boolean>})
{

    if(!chat_room)
    {
        return <div></div>;
    }

    return (
        <div className="w-full h-[10vh]">
            <div className="flex items-center justify-between p-6 border-b-2">
                <div className="flex items-center">
                    <img src={chat_room?.RoomImageUrl} alt={chat_room?.RoomName} className="w-12 h-12 rounded-full"/>
                    <div className="ml-3">
                        <h1 className="font-bold">{chat_room?.RoomName}</h1>
                        <div >
                            {
                                Array.from(onlineMap.values()).filter(function(item){
                                    return item;
                                }).length === 0 ?
                                    (<div className="flex"><GoDotFill className="text-gray-500 text-2xl"/>
                                        <p className="text-sm text-gray-500 pt-[2px]">Offline</p></div>)
                                    :
                                    (<div className="flex"><GoDotFill className="text-green-600 text-2xl"/>
                                        <p className="text-sm text-gray-500 pt-[2px]">{Array.from(onlineMap.values()).filter(function(item){
                                            return item;
                                        }).length > 1 ? Array.from(onlineMap.values()).filter(function(item){
                                            return item;
                                        }).length : "" } Online</p></div>)
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
                        <BsThreeDots className="text-2xl opacity-50"/>
                    </button>
                </div>
            </div>
        </div>
    );
}

export default ChatBoxHeader;
