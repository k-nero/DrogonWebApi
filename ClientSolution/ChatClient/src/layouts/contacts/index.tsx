import React from "react";
import Navbar from "@/components/navbar";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import { NavLink } from "react-router-dom";
import { IoPersonAddSharp } from "react-icons/io5";
import { Modal } from "antd";

const contact_links = [

    {
        name: "All",
        link: "/peoples/contact",
    },
    {
        name: "Suggested",
        link: "/peoples/suggested",
    },
    {
        name: "Friends Request Received",
        link: "/peoples/friends-request-received",
    },
    {
        name: "Friends Request Sent",
        link: "/peoples/friends-request-sent",
    },
    {
        name: "Blocked",
        link: "/peoples/blocked",
    }
];

function ContactLayout({ children }: { children: React.ReactNode })
{

    function ContactLinks()
    {
        return contact_links.map((links, index) => {
            return (
                <NavLink key={index} onClick={()=> {}} className={`flex items-center justify-between p-3 ${index === (contact_links.length - 1) ? "" : "border-b-2" }`} to={links.link}>
                    <div className="flex items-center">
                        <div className="ml-3">
                            <h1 className="font-bold">{links.name}</h1>
                        </div>
                    </div>
                </NavLink>
            );
        });
    }

    const [addContactModal, setAddContactModal] = React.useState(false);
    const [search, setSearch] = React.useState("");

    function handleSearchChange(e: React.ChangeEvent<HTMLInputElement>)
    {
        setSearch(e.target.value);
    }

    return (
        <>
            <Modal title="Add new contact" open={addContactModal} onOk={() => {
                setAddContactModal(false);
            }} onCancel={() => {
                setAddContactModal(false);
            }} footer={[]}
            centered={true} maskClosable={true} closable={true}
            >
                <div>
                    <InputField
                        variant=""
                        extra="mb-2"
                        placeholder="Search for people"
                        type="text"
                        label={""}
                        id={""}
                        onChange={handleSearchChange}
                    />
                    <p className={"text-gray-500 text-sm"}>Search for people using their username or email address</p>
                </div>

            </Modal>
            <div className="grid grid-cols-12">
                <Navbar/>
                <div className="min-h-screen col-span-11">
                    <div className="grid grid-cols-12">
                        <div className="p-5 col-span-3 border-r-2 min-h-screen max-h-screen">
                            <div className="flex justify-between">
                                <h1 className="font-bold text-2xl">People</h1>
                                <button>
                                    <BsThreeDots className="text-2xl opacity-50"/>
                                </button>
                            </div>
                            {/*<div className="">*/}
                            {/*    <InputField*/}
                            {/*        variant="auth"*/}
                            {/*        extra="mb-3"*/}
                            {/*        placeholder="Search for people"*/}
                            {/*        type="text"*/}
                            {/*        label={""}*/}
                            {/*        id={""}*/}
                            {/*    />*/}
                            {/*</div>*/}
                            <div>
                                <button className="text-center flex gap-2.5 justify-center bg-teal-500 w-full p-3 mt-4 rounded-full text-white font-bold"
                                onClick={() => {
                                    setAddContactModal(true);
                                }}
                                >
                                    <IoPersonAddSharp className="text-xl" />
                                    Add new contact
                                </button>
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
