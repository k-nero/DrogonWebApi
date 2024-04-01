import React from "react";
import Navbar from "@/components/navbar";

function ContactLayout({ children }: { children: React.ReactNode })
{
    return (
        <>
            <div className="grid grid-cols-12 w-full h-full">
                <Navbar/>
                {children}
            </div>
        </>
    );
}

export default ContactLayout;
