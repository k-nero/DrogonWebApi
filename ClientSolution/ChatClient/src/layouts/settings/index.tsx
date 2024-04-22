import Navbar from "@/components/navbar";
import React, { ReactNode } from "react";
import { BsThreeDots } from "react-icons/bs";
import InputField from "@/components/form/input.tsx";
import { NavLink } from "react-router-dom";

function SettingsLayout({ children }: { children: ReactNode })
{
    const settings = [
        {
            name: "General",
            link: "/settings/general"
        },
        {
            name: "Notifications",
            link: "/settings/notifications"
        },
        {
            name: "Code view",
            link: "/settings/code-view"
        }
    ];

    function SettingLink()
    {
        return settings.map((links, index) => {
            return (
                <NavLink key={index} onClick={() => {}} className={`flex items-center justify-between p-3 ${index === (settings.length - 1) ? "" : "border-b-2" }`} to={links.link}>
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
            <div className="grid grid-cols-12 w-full h-full">
                <Navbar/>
                <div className="min-h-screen col-span-11">
                    <div className="grid grid-cols-12">
                        <div className="p-5 col-span-3 border-r-2 min-h-screen max-h-screen">
                            <div className="flex justify-between">
                                <h1 className="font-bold text-2xl">Settings</h1>
                                <button>
                                    <BsThreeDots className="text-2xl opacity-50"/>
                                </button>
                            </div>
                            <div className="">
                                <InputField
                                    variant="auth"
                                    extra="mb-3"
                                    placeholder="Search for settings"
                                    type="text"
                                    label={""}
                                    id={""}
                                />
                            </div>
                            <div className="mt-4 h-[80%] overflow-y-auto">
                                <SettingLink/>
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

export default SettingsLayout;
