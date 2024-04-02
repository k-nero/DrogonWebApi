import React from "react";
import Navbar from "@/components/navbar";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import { NavLink } from "react-router-dom";


const contacts = [
    {
        id: "1",
        name: "John Doe",
        username: "@johndoe",
        avatar: "https://via.placeholder.com/150"
    },
    {
        id: "2",
        name: "John Doe",
        username: "@johndoe",
        avatar: "https://via.placeholder.com/150"

    }
];


function ContactLayout({ children }: { children: React.ReactNode })
{

    function ContactList()
    {
        return contacts.map((chat, index) => {
            return (
                <NavLink key={index} onClick={()=> {}} className="flex items-center justify-between p-3 border-b-2" to={`/contacts/${chat.id}`}>
                    <div className="flex items-center">
                        <img src={chat.avatar} alt={chat.name} className="w-12 h-12 rounded-full"/>
                        <div className="ml-3">
                            <h1 className="font-bold">{chat.name}</h1>
                            <p className="text-sm text-gray-500">{chat.username}</p>
                        </div>
                    </div>
                </NavLink>
            );
        });
    }

    return (
        <>
            <div className="grid grid-cols-12 w-full h-full">
                <Navbar/>
                <div className="min-h-screen col-span-11">
                    <div className="grid grid-cols-12">
                        <div className="p-5 col-span-3 min-h-screen border-r-2">
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
                            <div className="mt-4">
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
