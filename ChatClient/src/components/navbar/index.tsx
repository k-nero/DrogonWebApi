import { GoPeople, GoQuestion } from "react-icons/go";
import { IoSettingsOutline } from "react-icons/io5";
import React from "react";
import { NavLink, useLocation } from "react-router-dom";
import { AiOutlineAppstore, AiOutlineMessage } from "react-icons/ai";
import { CiLogout } from "react-icons/ci";
import { MdCamera } from "react-icons/md";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";

const navLinks = [
    {
        icon: AiOutlineAppstore,
        path: "/chats",
        name: "Chat"
    },
    {
        icon: GoPeople,
        path: "/peoples",
        name: "People"
    },
    {
        icon: AiOutlineMessage,
        path: "/messages",
        name: "Messages"
    },
    {
        icon: MdCamera ,
        path: "/moments",
        name: "Moments"
    },
    {
        icon: IoSettingsOutline,
        path: "/settings",
        name: "Settings"
    }
];


function Navbar()
{
    const location = useLocation();
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;

    if(!credential)
    {
        window.location.href = "/auth/sign-in";
    }

    function activeRoute(routeName: string)
    {
        return location.pathname.split("?")[0].startsWith(routeName);
    }

    function CreateNavLink()
    {
        return navLinks.map((link, index) => {
            if(activeRoute(link.path))
            {
                return (
                    <NavLink key={index} to={link.path} className="bg-gray-300 bg-opacity-30 rounded-full">
                        <link.icon className="block m-auto text-teal-600 "/>
                    </NavLink>
                );
            }
            else {
                return (
                    <NavLink key={index} to={link.path} className="" >
                        <link.icon className="block m-auto"/>
                    </NavLink>
                );
            }

        });
    }

    return (
        <div className="border-r-2 min-h-screen col-span-1 py-8 text-2xl">
            <div className="flex flex-col justify-between h-full">
                <div className="grid-cols-1 grid gap-y-8">
                    <button className="">
                        <img src={`${credential?.user?.AvatarUrl}`} alt="profile" className="block m-auto rounded-full w-12 h-12"/>
                    </button>
                    <CreateNavLink/>
                </div>
                <div className="grid-cols-1 grid gap-y-8">
                    <button>
                        <GoQuestion className="block m-auto"/>
                    </button>
                    <button>
                        <CiLogout className="block m-auto"/>
                    </button>
                </div>
            </div>
        </div>
    );
}

export default Navbar;
