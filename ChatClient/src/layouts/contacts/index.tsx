import React from "react";
import Navbar from "@/components/navbar";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import { NavLink } from "react-router-dom";

const contact_links = [

    {
        name: "All",
        link: "/contacts/all",
    },
    {
        name: "Suggested",
        link: "/contacts/suggested",
    },
    {
        name: "Friends Request Received",
        link: "/contacts/friends-request-received",
    },
    {
        name: "Friends Request Sent",
        link: "/contacts/friends-request-sent",
    },
    {
        name: "Blocked",
        link: "/contacts/blocked",
    }
];

function ContactLayout({ children }: { children: React.ReactNode })
{

    function ContactLinks()
    {
        return contact_links.map((links, index) => {
            return (
                <NavLink key={index} onClick={()=> {}} className="flex items-center justify-between p-3 border-b-2" to={links.link}>
                    <div className="flex items-center">
                        <div className="ml-3">
                            <h1 className="font-bold">{links.name}</h1>
                        </div>
                    </div>
                </NavLink>
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
                            <div className="mt-4 h-[80%] overflow-y-auto">
                                <ContactLinks/>
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
