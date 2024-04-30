import { AuthResponse } from "@/utils/type/AuthResponse.ts";

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);


async function Query<T>(endpoint: string, useToken: boolean = true): Promise<T>
{
    const credential: AuthResponse = JSON.parse(localStorage.getItem("auth_credential") || "{}" )

    if(!credential)
    {
        window.location.href = "/auth/sign-in";
    }

    //delay for 1 second
    //await new Promise((r) => setTimeout(r, 3000));

    const res = await fetch(`${baseUrl}${endpoint}`, {
        method: "GET",
        headers: {
            "Content-Type": "application/json",
            "Authorization": useToken ? `Bearer ${credential.access_token}` : ""
        },
    });
    if(res.status === 401)
    {
        localStorage.removeItem("auth_credential");
        window.location.href = "/auth/sign-in";
    }
    return await res.json();
}

export default Query;
