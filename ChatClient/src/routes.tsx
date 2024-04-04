import { createBrowserRouter } from "react-router-dom";
import SignIn from "./pages/auth/SignIn.tsx";
import ChatPage from "@/pages/chat";
import Contact from "@/pages/contact";
import Messages from "@/pages/messages";
import Moments from "@/pages/moments";
import Settings from "@/pages/settings";
import FriendRequestReceived from "@/pages/contact/friend-request-received";
import FriendRequestSent from "@/pages/contact/friend-request-sent";
import BlockedContact from "@/pages/contact/blocked";
import SuggestedContact from "@/pages/contact/suggested";
import ContactList from "@/pages/contact/contact-list";
import Chat from "@/pages/chat/chat";

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
        path: "/peoples",
        element: <Contact/>,
        children: [
            {
                name: "Contact",
                path: "/peoples/contact",
                element: <ContactList/>
            },
            {
                name: "Friends Request Received",
                path: "/peoples/friends-request-received",
                element: <FriendRequestReceived/>
            },
            {
                name: "Friends Request Sent",
                path: "/peoples/friends-request-sent",
                element: <FriendRequestSent/>
            },
            {
                name: "Blocked",
                path: "/peoples/blocked",
                element: <BlockedContact/>
            },
            {
                name: "Suggested",
                path: "/peoples/suggested",
                element: <SuggestedContact/>
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
                element: <Chat/>
            }
        ]
    }
];

const route = createBrowserRouter(routes);
export default route;
