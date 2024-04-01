import ChatLayout from "@/layouts/chats";
import { useLocation } from "react-router-dom";


function ChatPage()
{
    const location = useLocation();
    const chat_id = location.pathname.split("/")[2];

  return (
      <ChatLayout>
          <div>
              <h1>ChatBox id {chat_id}</h1>
          </div>
      </ChatLayout>
  );
}

export default ChatPage;
