import { Collapse } from "antd";
import { NavLink } from "react-router-dom";
import React, { useEffect, useState } from "react";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import Query from "@/utils/function/Query.ts";
import ContactListType from "@/utils/type/ContactListType.ts";
import PaginatedType from "@/utils/type/common/PaginatedType.ts";

function ContactList()
{

    const [localUser] = useLocalStorage("auth_credential", {});
    const user: AuthResponse = localUser;

    const [contactList, setContactList] = useState<ContactListType[]>([]);

    useEffect(() => {
        Query<PaginatedType<ContactListType>>(`/contact-list?page=1&limit=30&user_id=${user.user.Id}`).then((r) => {
            setContactList(r.m_data);
        });
    }, []);


    return contactList.map((contact_list) => {
        return (
            <Collapse key={contact_list.Id} defaultActiveKey={["1"]} bordered={false} ghost={true } expandIconPosition={"end"}>
                <Collapse.Panel header={<div className="text-xl font-bold">{contact_list.Name}</div>} key="1">
                    {contact_list.Contacts.map((contact, index) => {
                        return (
                            <NavLink key={contact.Id} to={`/peoples/contact/${contact.Id}`} className={`flex items-center p-3 ${index === contact_list.Contacts.length - 1 ? "" : "border-b-2" }`}>
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
