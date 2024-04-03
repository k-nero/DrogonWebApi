import InputField from "@/components/form/input.tsx";
import AuthLayout from "@/layouts/auth";
import GoogleLoginButton from "./GoogleLoginButton";
import { Checkbox } from "antd";
import React, { useState } from "react";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage";
const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);


function SignIn()
{
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [,setUser] = useLocalStorage("auth_credential", {});

    async function handleSubmit(e: React.MouseEvent<HTMLButtonElement>)
    {
        e.preventDefault();
        const res = await fetch(`${baseUrl}/auth/login`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify({
                username: email,
                password: password,
            }),
        });

        const data: AuthResponse = await res.json();
        if(res.status === 200)
        {
            setUser(data);
            window.location.href = "/chats";
        }
    }

    return (
        <AuthLayout>
            <div className="mb-16 mt-16 flex h-full w-full items-center justify-center px-2 md:mx-0 md:px-0 lg:mb-10 lg:items-center lg:justify-start">
                {/* Sign in section */}
                <div className="mt-[10vh] w-full max-w-full flex-col items-center md:pl-4 lg:pl-0 xl:max-w-[420px]">
                    <h4 className="mb-2.5 text-4xl font-bold text-navy-700 dark:text-white">
                        Sign In
                    </h4>
                    <p className="mb-9 ml-1 text-base text-gray-600">
                        Enter your email and password to sign in!
                    </p>
                    <GoogleLoginButton/>
                    <div className="mb-6 flex items-center  gap-3">
                        <div className="h-px w-full bg-gray-200 dark:bg-navy-700"/>
                        <p className="text-base text-gray-600 dark:text-white"> or </p>
                        <div className="h-px w-full bg-gray-200 dark:bg-navy-700"/>
                    </div>
                    <form action="#" onSubmit={(e) => e.preventDefault()}>
                        {/* Email */}
                        <InputField
                            required
                            variant="auth"
                            extra="mb-3"
                            label="Email*"
                            placeholder="mail@simmmple.com"
                            id="email"
                            type="text"
                            onChange={(e) => setEmail(e.target.value)}
                            // defaultValue="raekyo_admin@gmail.com"
                        />

                        {/* Password */}
                        <InputField
                            required
                            variant="auth"
                            extra="mb-3"
                            label="Password*"
                            placeholder="Min. 8 characters"
                            id="password"
                            type="password"
                            onChange={(e) => setPassword(e.target.value)}
                            // defaultValue="2024RaeKyoAdmin"
                        />
                        {/* Checkbox */}
                        <div className="mb-4 flex items-center justify-between px-2">
                            <div className="flex items-center">
                                <Checkbox/>
                                <p className="ml-2 text-sm font-medium text-navy-700 dark:text-white">
                                    Keep me logged In
                                </p>
                            </div>
                            <a
                                className="text-sm font-medium text-brand-500 hover:text-brand-600 dark:text-white"
                                href="#"
                            >
                                Forgot Password?
                            </a>
                        </div>
                        <button
                            type="submit"
                            onClick={(e) =>
                                handleSubmit(e)
                            }
                            className="linear mt-2 w-full rounded-xl bg-brand-500 py-[12px] text-base font-medium text-white transition duration-200 hover:bg-brand-600 active:bg-brand-700 dark:bg-brand-400 dark:text-white dark:hover:bg-brand-300 dark:active:bg-brand-200"
                        >
                            Sign In
                        </button>
                    </form>
                    <div className="mt-4">
            <span className=" text-sm font-medium text-navy-700 dark:text-gray-600">
              Not registered yet?
            </span>
                        <a
                            href="#"
                            className="ml-1 text-sm font-medium text-brand-500 hover:text-brand-600 dark:text-white"
                        >
                            Create an account
                        </a>
                    </div>
                </div>
            </div>
        </AuthLayout>
    );
}

export default SignIn;
