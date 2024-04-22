import Navbar from "@/components/navbar";
import React from "react";

function MessagesLayout({ children }: { children: React.ReactNode })
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

export default MessagesLayout;
