import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App.tsx";
import "./css/index.css";
import { RouterProvider } from "react-router-dom";
import router from "./routes";
import { ConfigProvider } from "antd";

ReactDOM.createRoot(document.getElementById("root")!).render(
    <>
        <ConfigProvider
        theme={{
            components: {
                Switch: {
                    colorPrimary: "rgb(20 184 166)",
                    colorPrimaryHover: "rgb(20 184 166)",
                }
            }
        }}
        >
            <App>
                <RouterProvider router={router}/>
            </App>
        </ConfigProvider>

    </>
);
