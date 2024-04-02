import ContactLayout from "@/layouts/contacts";
import { useLocation } from "react-router-dom";


const contact = {
    Id: "cd83762e-e7b2-4efd-b01e-055890be02cd",
    Tag: "Friends",
    ContactName: "John Doe",
    ApplicationUserId: "2b5c9ea4-ace9-437f-ba7a-d86e4a7de96c",
    ContactListId: "cd83762e-e7b2-4efd-b01e-055890be02cd",
    CreatedDate: "2024-04-02T13:41:31.677",
    ModifiedDate: "",
    ApplicationUser: {
        Id: "2b5c9ea4-ace9-437f-ba7a-d86e4a7de96c",
        UserName: "johndoe",
        PasswordHash: "AQAAAAEAACcQAAAAEJQb2Kz9jFzvV7QJ8ZzYvVZ3mZKk5q9ZzZQ5",
        Email: "",
        PhoneNumber: "",
        AvatarUrl: "https://via.placeholder.com/150",
        CreatedDate: "2024-04-02T13:41:31.677",
        ModifiedDate: "",
        ContactListId: "cd83762e-e7b2-4efd-b01e-055890be02cd",
        chatParticipants: [],
        ContactList: null,
    }
};


function Contact()
{
    const location = useLocation();
    const contact_id = location.pathname.split("/")[2];

    return (
        <ContactLayout>
            {
                contact_id ?
                <div className="flex items-center justify-center h-full">
                    <div className="flex items-center">
                         <img src={contact.ApplicationUser.AvatarUrl} alt={contact.ContactName} className="w-32 h-32 m-auto rounded-full"/>
                        <div className="ml-4">
                            <h1 className="text-2xl">{contact.ContactName}</h1>
                            <p className="text-gray-500">@{contact.ApplicationUser.UserName}</p>
                        </div>
                    </div>



                </div>
                    :
                    <div className="flex items-center justify-center h-full">
                        <h1 className="text-2xl">Select a contact to view details</h1>
                    </div>
            }
        </ContactLayout>
    );
}

export default Contact;
