import MessageType from "../type/MessageType";

interface SocketMessageType
{
    type: "message";
    message: MessageType;
    channel: string;
}

export default SocketMessageType;
