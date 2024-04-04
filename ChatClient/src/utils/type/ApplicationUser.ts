import BaseType from "@/utils/type/BaseType.ts";

interface ApplicationUser extends BaseType
{
    UserName: string;
    PasswordHash: string;
    Email: string;
    PhoneNumber?: string;
    AvatarUrl?: string;
    chatParticipants: never[],
    ContactList: never,
}

export default ApplicationUser;
