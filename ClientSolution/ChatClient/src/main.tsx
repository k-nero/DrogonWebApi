import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App.tsx";
import "./css/index.css";
import { RouterProvider } from "react-router-dom";
import router from "./routes";

ReactDOM.createRoot(document.getElementById("root")!).render(
    <>
        <App>
            <RouterProvider router={router}/>
        </App>
    </>
);
