import { GoDotFill } from "react-icons/go";
import { BiSolidPhoneCall } from "react-icons/bi";
import React from "react";
import { BsCameraVideoFill, BsThreeDots } from "react-icons/bs";

function ChatBoxHeader({setIsPanel} : {setIsPanel: () => void})
{
    return (
        <div className="w-full h-[10%]">
            <div className="flex items-center justify-between p-6 border-b-2">
                <div className="flex items-center">
                    <img src="https://via.placeholder.com/150" alt="John Doe" className="w-12 h-12 rounded-full"/>
                    <div className="ml-3">
                        <h1 className="font-bold">John Doe</h1>
                        <div className="flex">
                            <GoDotFill className="text-green-600 text-2xl"/>
                            <p className="text-sm text-gray-500 pt-[2px]">Online</p>
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
