import React, { useEffect, useState } from "react";
import MessageType from "@/utils/type/MessageType.ts";
import Query from "@/utils/function/Query.ts";
import AttachView from "@/pages/chat/chat/components/message/AttachView.tsx";
import CodeView from "@/components/text/CodeMessage.tsx";
import Text from "@/components/text/Text.tsx";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { Skeleton } from "antd";
import { IoArrowRedoSharp } from "react-icons/io5";
import messageBox from "@/pages/chat/chat/components/MessageBox.tsx";

function QuoteMessage({ message, incoming }: { message: MessageType, incoming: boolean }) {

    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;

    const [quoteMessage, setQuoteMessage] = useState<MessageType>();
    if (!message.QuoteMessageId)
    {
        return null;
    }
    useEffect(() => {
        Query<MessageType>(`/message/${message.QuoteMessageId}`).then((r) => {
            setQuoteMessage(r);
        });
    }, []);

    if (incoming)
    {
        return (
            <div className="">
                <button onClick={(e) => {
                    if(message.QuoteMessageId)
                    {
                        const elem = document.getElementById(message.QuoteMessageId);
                        if(elem)
                        {
                            elem.scrollIntoView({ behavior: "smooth" , block: "center"});
                        }
                    }
                }}>
                    <p className="text-xs mr-auto w-fit opacity-70 px-3 hover:underline">
                        <IoArrowRedoSharp className="inline-block opacity-70"/> Replying to {
                        quoteMessage?.ApplicationUser?.UserName === credential.user.UserName ? "You" : "@" + quoteMessage?.ApplicationUser?.UserName
                    }</p>
                </button>
                <div className="opacity-70 bg-white p-3 mx-3 rounded-xl max-w-96 ">
                    {quoteMessage ?
                        <div>
                            <AttachView message={quoteMessage}/>
                        </div>
                        : <Skeleton active className="p-3"/>}
                    <div className={`max-h-96 overflow-auto ${quoteMessage?.MessageAttachs?.length && quoteMessage.TextMessage ? "mt-3" : ""}`}>{
                        quoteMessage ?
                            <>
                                {
                                    quoteMessage.TextMessage?.startsWith("```") ?
                                        <CodeView textMessage={quoteMessage.TextMessage}/>
                                        : <><Text text={quoteMessage.TextMessage}/></>
                                }
                            </> : <Skeleton active/>
                    }</div>
                </div>
            </div>
        );
    }
    else
    {
        return (
            <div className="">
                <button onClick={(e) => {
                    if(message.QuoteMessageId)
                    {
                        const elem = document.getElementById(message.QuoteMessageId);
                        if(elem)
                        {
                            elem.scrollIntoView({ behavior: "smooth", block: "center" });
                        }
                    }

                }}>
                    <p className="text-xs ml-auto w-fit opacity-70 px-3 hover:underline">
                        <IoArrowRedoSharp className="inline-block opacity-70"/> Replying to {
                        quoteMessage?.ApplicationUser?.UserName === credential.user.UserName ? "You" : "@" + quoteMessage?.ApplicationUser?.UserName
                    }</p>
                </button>
                <div className="opacity-70 bg-white p-3 mx-3 rounded-xl max-w-96 ">
                    {quoteMessage ?
                        <div>
                            <AttachView message={quoteMessage}/>
                        </div>
                        : <Skeleton.Image active/>}
                    <div className={`max-h-96 overflow-auto text-sm break-words ${quoteMessage?.MessageAttachs?.length && quoteMessage.TextMessage ? "mt-3" : ""}`}>{
                        quoteMessage ?
                        <>
                            {
                                quoteMessage.TextMessage?.startsWith("```") ?
                                    <CodeView textMessage={quoteMessage.TextMessage}/>
                                    : <>
                                        <Text text={quoteMessage.TextMessage}/>
                                    </>
                            }
                        </> : <Skeleton active/>
                    }</div>
                </div>
            </div>
        );
    }
}

export default QuoteMessage;
