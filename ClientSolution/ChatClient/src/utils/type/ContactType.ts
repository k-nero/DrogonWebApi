import ApplicationUser from "./ApplicationUser";
import ContactListType from "./ContactListType";
import BaseType from "@/utils/type/BaseType.ts";

interface ContactType extends BaseType
{
    ApplicationUserId: string;
    ContactListId: string;
    Tag: string;

    ApplicationUser: ApplicationUser;
    ContactList: ContactListType;
}

export default ContactType;


