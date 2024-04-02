
import { TfiClose } from "react-icons/tfi";
import { Collapse, CollapseProps, Switch } from "antd";
import { BsThreeDots } from "react-icons/bs";
import React from "react";

const fileList = (
    <div>
        <div className="flex justify-between">
            <h1 className="">FileName</h1>
            <button>
                <BsThreeDots className="text-2xl opacity-50"/>
            </button>
        </div>
        <div className="flex justify-between">
            <h1 className="">FileName</h1>
            <button>
                <BsThreeDots className="text-2xl opacity-50"/>
            </button>
        </div>
        <div className="flex justify-between">
            <h1 className="">FileName</h1>
            <button>
                <BsThreeDots className="text-2xl opacity-50"/>
            </button>
        </div>
        <div className="flex justify-between">
            <h1 className="">FileName</h1>
            <button>
            <BsThreeDots className="text-2xl opacity-50"/>
        </button>

        </div>
    </div>
);

const image = (
    <div className="grid-cols-3 grid gap-y-4">
        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 m-auto"/>
        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 m-auto"/>
        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 m-auto"/>
        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 m-auto"/>
        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 m-auto"/>
        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 m-auto"/>
    </div>
);

const items: CollapseProps["items"] = [
    {
        key: "file",
        label: <p className="font-bold text-[15px] py-3 ">Recent Files</p>,
        children: fileList,
        showArrow: false
    },
    {
        key: "image",
        label: <p className="font-bold text-[15px] py-3">Recent Images</p>,
        children: image,
        showArrow: false,
    },


];

function ChatPanel({ setIsPanel }: { setIsPanel: () => void })
{
    return (
        <div className="p-6 border-l-2 h-full">
            <div className="text-end">
                <button className="p-3 rounded-full border-2 w-fit" onClick={setIsPanel}>
                    <TfiClose  className=" text-1xl "/>
                </button>
            </div>
            <div className="mt-8 text-center border-b-2 pb-8">
                <img src="https://via.placeholder.com/150" alt="John Doe" className="w-32 h-32 rounded-full m-auto border-4 border-teal-500"/>
                <h1 className="mt-4 font-bold text-2xl">John Doe</h1>
                <p className="text-sm text-gray-500">@john_doe</p>
            </div>
            <div className="flex justify-between py-6 px-4 border-b-2">
                <h1 className="font-bold">Notifications</h1>
                <Switch className="text-teal-500" />
            </div>
            <Collapse activeKey={["file", "image"]} items={items} className="text-teal-500 bg-white text-1xl border-b-2" bordered={false}/>
        </div>
    );
}

export default ChatPanel;
