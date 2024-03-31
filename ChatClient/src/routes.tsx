import { createBrowserRouter } from "react-router-dom";
import SignIn from "./pages/auth/SignIn.tsx";
import ChatPage from "@/pages/chat";
import ChatRoom from "@/pages/chat/room";

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
        element: <SignIn />
    },
    {
        name: "Chat",
        path: "/chat",
        element: <ChatPage />,
        children: [
            {
                name: "Chat",
                path: "/chat/:chat_id",
                element: <ChatRoom />
            }
        ]
    }
];

const route = createBrowserRouter(routes);
export default route;
