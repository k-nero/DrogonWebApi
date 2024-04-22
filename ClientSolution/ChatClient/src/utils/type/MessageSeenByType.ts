import ApplicationUser from "./ApplicationUser";
import BaseType from "./BaseType";
import MessageType from "@/utils/type/MessageType.ts";

interface MessageSeenByType extends BaseType
{

    MessageId: string;
    ApplicationUserId: string;

    Message: MessageType;
    ApplicationUser: ApplicationUser;
}

export default MessageSeenByType;
