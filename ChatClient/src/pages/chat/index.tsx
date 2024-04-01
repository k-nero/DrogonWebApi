import ChatLayout from "@/layouts/chats";
import { useLocation } from "react-router-dom";
import ChatBoxHeader from "@/pages/chat/components/ChatBoxHeader.tsx";
import MessageBox from "@/pages/chat/components/MessageBox.tsx";


function ChatPage()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

  return (
      <ChatLayout>
          <ChatBoxHeader/>
          <div className="w-fit h-fit">
              <MessageBox/>
          </div>
      </ChatLayout>
  );
}

export default ChatPage;
