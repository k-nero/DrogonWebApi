import React from "react";
import Navbar from "@/components/navbar";
import useLocalStorage from "@/utils/hooks/useLocalStorage";

function ChatLayout({ children }: { children: React.ReactNode })
{
    const [userLocal, setUser] = useLocalStorage("auth_credential", {});

    return (
        <div className="grid grid-cols-12 w-full h-full max-h-screen">
            <Navbar/>
            {children}
        </div>
    );
}

export default ChatLayout;
