import { Collapse } from "antd";
import { NavLink } from "react-router-dom";
import React from "react";

const contactList = [
    {
        Id: "cd83762e-e7b2-4efd-b01e-055890be02cd",
        Name: "Friends",
        ApplicationUserId: "2b5c9ea4-ace9-437f-ba7a-d86e4a7de96c",
        CreatedDate: "2024-04-02T13:41:31.677",
        ModifiedDate: "",
        Contacts: [
            {
                Id: "cd83762e-e7b2-4efd-b01e-055890be02cd",
                Tag: "Friends",
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
            },
            {
                Id: "2b5c9ea4-ace9-437f-ba7a-d86e4a7de96c",
                Tag: "Friends",
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
            }
        ]
    }
];

function ContactList()
{
    return contactList.map((contact_list) => {
        return (
            <Collapse key={contact_list.Id} defaultActiveKey={["1"]} bordered={false} ghost={true } expandIconPosition={"end"}>
                <Collapse.Panel header={<div className="text-xl font-bold">{contact_list.Name}</div>} key="1">
                    {contact_list.Contacts.map((contact, index) => {
                        return (
                            <NavLink key={contact.Id} to={`/contacts/${contact.Id}`} className={`flex items-center p-3 ${index === contact_list.Contacts.length - 1 ? "" : "border-b-2" }`}>
                                <img src={contact.ApplicationUser.AvatarUrl} alt={contact.ApplicationUser.UserName} className="w-12 h-12 rounded-full"/>
                                <div className="ml-3">
                                    <h1 className="font-bold">{contact.ApplicationUser.UserName}</h1>
                                    <p className="text-sm text-gray-500">@{contact.ApplicationUser.UserName}</p>
                                </div>
                            </NavLink>
                        );
                    })}
                </Collapse.Panel>
            </Collapse>
        );
    });
}

export default ContactList;
