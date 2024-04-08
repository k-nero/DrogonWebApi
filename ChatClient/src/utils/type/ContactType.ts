import ApplicationUser from "./ApplicationUser";
import ContactListType from "./ContactListType";

interface ContactType
{
    ApplicationUserId: string;
    ContactListId: string;
    Tag: string;

    ApplicationUser: ApplicationUser;
    ContactList: ContactListType;
}

export default ContactType;


