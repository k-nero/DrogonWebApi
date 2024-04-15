import MessageType from "@/utils/type/MessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { IoCheckmarkDoneOutline } from "react-icons/io5";

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
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96  ">
                            <p className="text-sm break-words">{message.TextMessage}</p>
                        </div>
                    </div>
                </div>
                <div className="flex gap-2.5">
                    <p className="text-xs text-gray-500 text-left ml-9 p-1">{
                        new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                            hour: "2-digit",
                            minute: "2-digit"
                        })
                    }</p>
                    {
                        message.MessageSeenBys?.length && message.MessageSeenBys?.length > 0 ?
                            <p className="text-md text-gray-700 text-left p-1">
                                <IoCheckmarkDoneOutline/>
                            </p>
                            : null
                    }
                </div>
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
                            <p className="text-sm break-words">{message.TextMessage}</p>
                        </div>
                    </div>
                    <div className="mt-auto">
                        <img src={message?.ApplicationUser?.AvatarUrl} alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                </div>
                <div className="flex justify-end gap-2.5">
                    {
                        message.MessageSeenBys?.length && message.MessageSeenBys?.length > 0 ?
                        <p className="text-md text-gray-700 text-left p-1">
                            <IoCheckmarkDoneOutline/>
                        </p>
                            : null
                    }
                    <p className="text-xs text-gray-500 text-right mr-9 p-1">{
                        new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                            hour: "2-digit",
                            minute: "2-digit"
                        })
                    }</p>
                </div>

            </div>
        );
    }
}

export default Message;
