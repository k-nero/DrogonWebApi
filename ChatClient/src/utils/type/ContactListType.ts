import ContactType from "@/utils/type/ContactType.ts";

interface ContactListType
{
    ApplicationUserId: string;
    Name: string;

    Contacts: ContactType[];
}


export default ContactListType;
