import React from "react";
import Navbar from "@/components/navbar";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import { NavLink } from "react-router-dom";
import { Collapse } from "antd";


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
            }
        ]
    },
    {
        Id: "cd83762e-e7b2-4efd-b01e-055890be02cd",
        Name: "Colleague",
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
                    Id: "cd83762e-e7b2-4efd-b01e-055890be02cd",
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


function ContactLayout({ children }: { children: React.ReactNode })
{

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

    return (
        <>
            <div className="grid grid-cols-12">
                <Navbar/>
                <div className="min-h-screen col-span-11">
                    <div className="grid grid-cols-12">
                        <div className="p-5 col-span-3 border-r-2 min-h-screen max-h-screen">
                            <div className="flex justify-between">
                                <h1 className="font-bold text-2xl">Contacts</h1>
                                <button>
                                    <BsThreeDots className="text-2xl opacity-50"/>
                                </button>
                            </div>
                            <div className="">
                                <InputField
                                    variant="auth"
                                    extra="mb-3"
                                    placeholder="Search for contact"
                                    type="text"
                                    label={""}
                                    id={""}
                                />
                            </div>
                            <div>
                                <button className="text-center bg-teal-500 w-full p-4 rounded-full text-white font-bold">
                                    Add contact
                                </button>
                            </div>
                            <div className="mt-4 h-[80%] overflow-y-auto">
                                <ContactList/>
                            </div>
                        </div>
                        <div className="col-span-9">
                            {children}
                        </div>
                    </div>
                </div>
            </div>
        </>
    );
}

export default ContactLayout;
