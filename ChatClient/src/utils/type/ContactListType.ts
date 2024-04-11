import ContactType from "@/utils/type/ContactType.ts";
import BaseType from "@/utils/type/BaseType.ts";

interface ContactListType extends BaseType
{
    ApplicationUserId: string;
    Name: string;

    Contacts: ContactType[];
}


export default ContactListType;
