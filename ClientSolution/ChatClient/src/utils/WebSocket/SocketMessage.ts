import MessageType from "@/utils/type/MessageType.ts";

interface SocketMessage<T>
{
    type: string;
    message: T;
    channel: string;
}

export default SocketMessage;
