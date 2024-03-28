import app from "@/firebase";
import { signInWithPopup, GoogleAuthProvider, getAuth } from "firebase/auth";
import { FcGoogle } from "react-icons/fc";
import { useNavigate } from "react-router-dom";

const firebaseAuth = getAuth(app);
const provider = new GoogleAuthProvider();

const GoogleLoginButton = () => {
  const navigate = useNavigate();
  // const [auth, setAuth] = useLocalStorage<object>("auth", {});

  function handleSignIn() {
    signInWithPopup(firebaseAuth, provider)
      .then(async (res) => {
        const user = res.user;
          // eslint-disable-next-line @typescript-eslint/no-unused-vars
        const idToken = await user.getIdToken();

        navigate("/");
      })
      .catch((error) => {
        const errorCode = error.code;
        const errorMessage = error.message;
        const credential = GoogleAuthProvider.credentialFromError(error);

        console.error(`ERROR: ${errorCode} - ${errorMessage} - ${credential} `);
      });
  }

  return (
    <>
      <button
        onClick={handleSignIn}
        className="mb-6 flex h-[50px] w-full items-center justify-center gap-2 rounded-xl bg-lightPrimary hover:cursor-pointer dark:bg-navy-800 hover:scale-110"
      >
        <div className="rounded-full text-xl">
          <FcGoogle />
        </div>
        <h5 className="text-sm font-medium text-navy-700 dark:text-white">
          Sign In with Google
        </h5>
      </button>
    </>
  );
};

export default GoogleLoginButton;
