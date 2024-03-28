import { createBrowserRouter } from "react-router-dom";
import SignIn from "./pages/auth/SignIn.tsx";

export type RoutesType = {
    name: string;
    path: string;
    element: JSX.Element;
    errorElement?: JSX.Element;
};


const routes: RoutesType[] = [
    {
        name: "Authentication",
        path: "/auth/sign-in",
        element: <SignIn />
    }
];

const route = createBrowserRouter(routes);
export default route;
