/* eslint-disable @typescript-eslint/no-unused-vars */
import app from "@/firebase";
import { Authentication } from "@/utils/function/Auth.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage";
import { getAuth, GoogleAuthProvider, signInWithPopup } from "firebase/auth";
import { FcGoogle } from "react-icons/fc";
import { useNavigate } from "react-router-dom";

const firebaseAuth = getAuth(app);
const provider = new GoogleAuthProvider();

function GoogleLoginButton()
{
    const navigate = useNavigate();
    const [userLocal, setUser] = useLocalStorage("user", {});

    // Simplified handleSignIn function
    async function handleSignIn()
    {
        const res = await signInWithPopup(firebaseAuth, provider);
        const user = res.user;
        const idToken = await user.getIdToken();
        console.log({
            idToken
        });
        const accountResponse: AuthResponse = await Authentication(idToken);
        setUser(accountResponse.user);
        console.log({
            accessToken: accountResponse.access_token
        });

        alert(`Login successfully, Welcome back: ${accountResponse.user.UserName} 👋`);
        navigate("/");
    }

    return (
        <>
            <button
                onClick={handleSignIn}
                className="mb-6 flex h-[50px] w-full items-center justify-center gap-2 rounded-xl bg-lightPrimary hover:scale-110 hover:cursor-pointer dark:bg-navy-800"
            >
                <div className="rounded-full text-xl">
                    <FcGoogle/>
                </div>
                <h5 className="text-sm font-medium text-navy-700 dark:text-white">
                    Sign In with Google
                </h5>
            </button>
        </>
    );
}

export default GoogleLoginButton;
