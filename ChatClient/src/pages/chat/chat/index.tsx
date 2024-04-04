import { useLocation } from "react-router-dom";
import { useState } from "react";
import ChatBoxHeader from "@/pages/chat/chat/components/ChatBoxHeader.tsx";
import MessageBox from "@/pages/chat/chat/components/MessageBox.tsx";
import MessageInput from "@/pages/chat/chat/components/MessageInput.tsx";
import ChatPanel from "@/pages/chat/chat/components/ChatPanel.tsx";


function Chat()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [isPanelOpen, setIsPanelOpen] = useState(false);


    function setIsPanel()
    {
        setIsPanelOpen(!isPanelOpen);
    }

    return (
        <>
            {
                chat_id ?
                    <div className={`w-full h-full ${isPanelOpen ? "grid grid-cols-10" : ""}`}>
                        <div className={`w-full h-full ${isPanelOpen ? "col-span-6" : ""}`}>
                            <ChatBoxHeader setIsPanel={setIsPanel}/>
                            <div className="bg-gray-100 bg-opacity-90 p-4 flex flex-col justify-between h-[90%]">
                                <MessageBox/>
                                <MessageInput/>
                            </div>
                        </div>
                        {
                            isPanelOpen &&
                            <div className="col-span-4">
                                <ChatPanel setIsPanel={setIsPanel}/>
                            </div>
                        }
                    </div>
                    :
                    <div className="flex items-center justify-center h-full">
                        <h1 className="text-2xl ">Select a chat to start conversion </h1>
                    </div>
            }
        </>
    );
}

export default Chat;
