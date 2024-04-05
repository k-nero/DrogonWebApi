import MessageType from "@/utils/type/MessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";

function Message({message} : {message: MessageType})
{

    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;

    const incoming = message.ApplicationUserId !== credential.user.Id;

    if(incoming)
    {
        return (
            <div>
                <div className="w-full flex">
                    <div className="mt-auto">
                        <img src={message?.ApplicationUser?.AvatarUrl} alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                    <div>
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96">
                            <p className="text-sm">{message.TextMessage}</p>
                        </div>
                    </div>
                </div>
                <p className="text-xs text-gray-500 text-left ml-9 p-1">{
                    new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                        hour: "2-digit",
                        minute: "2-digit"
                    })
                }</p>
            </div>
        );
    }
    else
    {
        return (
            <div>
                <div className="w-full flex justify-end">
                    <div>
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96">
                            <p className="text-sm">{message.TextMessage}</p>
                        </div>
                    </div>
                    <div className="mt-auto">
                        <img src={message?.ApplicationUser?.AvatarUrl} alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                </div>
                <p className="text-xs text-gray-500 text-right mr-9 p-1">{
                    new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                        hour: "2-digit",
                        minute: "2-digit"
                    })
                }</p>
            </div>
        );
    }
}

export default Message;
