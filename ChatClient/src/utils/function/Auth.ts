const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

async function Authentication(idToken: string)
{
    try
    {
        const url: URL = new URL(`${baseUrl}/auth`);
        const res = await fetch(url, {
            method: "POST",
            // mode: "no-cors",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                idToken: idToken
            })
        });

        return await res.json();
    }
    catch (error)
    {
        throw new Error(`${error}`);
    }
}

export { Authentication };
