import { createBrowserRouter } from "react-router-dom";
import SignIn from "./pages/auth/SignIn.tsx";
import ChatPage from "@/pages/chat";
import Contact from "@/pages/contact";
import Messages from "@/pages/messages";
import Moments from "@/pages/moments";
import Settings from "@/pages/settings";

export type RoutesType = {
    name: string;
    path: string;
    element: JSX.Element;
    errorElement?: JSX.Element;
    children?: RoutesType[];
};


const routes: RoutesType[] = [
    {
        name: "Authentication",
        path: "/auth/sign-in",
        element: <SignIn/>
    },
    {
        name: "Contact",
        path: "/contacts",
        element: <Contact/>,
        children: [
            {
                name: "Contact",
                path: "/contacts/:id",
                element: <Contact/>
            }
        ]
    },
    {
        name: "Messages",
        path: "/messages",
        element: <Messages/>,
    },
    {
        name: "Moment",
        path: "/moments",
        element: <Moments/>,
    },
    {
        name: "Settings",
        path: "/settings",
        element: <Settings/>,
    },
    {
        name: "Chat",
        path: "/chats",
        element: <ChatPage/>,
        children: [
            {
                name: "Chat",
                path: "/chats/:id",
                element: <ChatPage/>
            }
        ]
    }
];

const route = createBrowserRouter(routes);
export default route;
