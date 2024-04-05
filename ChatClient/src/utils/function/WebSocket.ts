import { AuthResponse } from "@/utils/type/AuthResponse.ts";

const auth_credential: AuthResponse = JSON.parse(localStorage.getItem("auth_credential") || "{}");

if (!auth_credential) {
    window.location.href = "/auth/sign-in";
}

const webSocket = new WebSocket("wss://localhost:9001?authorization=" + auth_credential.access_token);
webSocket.onopen = () => {
    console.log("WebSocket 连接成功");
};

webSocket.onmessage = (event) => {
    console.log("WebSocket 收到消息", event.data);
};

webSocket.onclose = () => {
    console.log("WebSocket 连接关闭");
};

webSocket.onerror = (error) => {
    console.log("WebSocket 连接出错", error);
};

export default webSocket;
