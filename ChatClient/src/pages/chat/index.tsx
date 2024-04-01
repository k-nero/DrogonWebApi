import ChatLayout from "@/layouts/chats";
import { useLocation } from "react-router-dom";
import ChatBoxHeader from "@/pages/chat/components/ChatBoxHeader.tsx";
import MessageBox from "@/pages/chat/components/MessageBox.tsx";
import MessageInput from "@/pages/chat/components/MessageInput.tsx";
import { useState } from "react";
import ChatPanel from "@/pages/chat/components/ChatPanel.tsx";


function ChatPage()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];
    const [isPanelOpen, setIsPanelOpen] = useState(false);

    function setIsPanel()
    {
        setIsPanelOpen(!isPanelOpen);
    }

    return (
        <ChatLayout>
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

        </ChatLayout>
    );
}

export default ChatPage;
