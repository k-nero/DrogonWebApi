import BaseType from "@/utils/type/BaseType.ts";
import ContactListType from "@/utils/type/ContactListType.ts";
import ChatParticipant from "@/utils/type/ChatParticipant.ts";

interface ApplicationUser extends BaseType
{
    UserName: string;
    PasswordHash: string;
    Email: string;
    PhoneNumber?: string;
    AvatarUrl?: string;

    ChatParticipants: ChatParticipant[],
    ContactList: ContactListType[];
}

export default ApplicationUser;
