import ContactLayout from "@/layouts/contacts";
import { Outlet, redirect } from "react-router-dom";
import React from "react";

function Contact()
{
    return (
        <ContactLayout>
           <Outlet/>
        </ContactLayout>
    );
}

export default Contact;
