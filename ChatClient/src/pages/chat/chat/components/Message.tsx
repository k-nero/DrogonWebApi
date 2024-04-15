import MessageType from "@/utils/type/MessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { IoArrowRedoSharp, IoArrowUndo, IoCheckmarkDoneOutline } from "react-icons/io5";
import { MdOutlineEmojiEmotions } from "react-icons/md";
import { Tooltip } from "antd";
import React, { useEffect, useState } from "react";
import EmojiPicker, { EmojiClickData, EmojiStyle } from "emoji-picker-react";
import { BsThreeDots } from "react-icons/bs";
import Query from "@/utils/function/Query.ts";
import { Guid } from "guid-typescript";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import SocketMessage from "@/utils/WebSocket/SocketMessage.ts";
import MessageReactionType from "@/utils/type/MessageReactionType.ts";
const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function Message({message} : {message: MessageType})
{
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;
    //const [mess, setMessage] = useState<MessageType>(message);
    const incoming = message.ApplicationUserId !== credential.user.Id;

    function EmojiClickCallback(emoji: EmojiClickData, event: MouseEvent)
    {
        const reaction = message.MessageReactions?.find((r) => r.ApplicationUserId === credential.user.Id && r.ReactionType === emoji.emoji);
        if(!reaction)
        {
            fetch(`${baseUrl}/message-reaction`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify({
                    MessageId: message.Id,
                    ReactionType: emoji.emoji,
                    ApplicationUserId: credential.user.Id,
                    ReactionUrl: emoji.imageUrl,
                    ReactionCount: 1
                })
            }).then((r) => {
                if(r.ok)
                {
                    r.json().then((rs) => {
                        Query<MessageReactionType>(`/message-reaction/${rs.id}`).then((mrs) => {
                            uWebSockets.getInstance().send(JSON.stringify({
                                type: "reaction",
                                channel: message.ChatRoomId,
                                message: mrs
                            }));
                        })
                    })
                }
            })
        }
        else
        {
            reaction.ReactionCount += 1;
            fetch(`${baseUrl}/message-reaction/${reaction.Id}`, {
                method: "PATCH",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify(reaction)
            }).then((r) => {})
        }
    }

    function EmojiTooltip()
    {
        return (
            <EmojiPicker onReactionClick={(emoji, event) => EmojiClickCallback(emoji, event) }  reactionsDefaultOpen={true} emojiStyle={EmojiStyle.FACEBOOK} />
        );
    }


    if(incoming)
    {
        return (
            <div>
                <div className="w-full flex group " >
                    <div className="mt-auto">
                        <img src={message?.ApplicationUser?.AvatarUrl} alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                    <div className="group">
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96 relative">
                            <p className="text-sm break-words">{message.TextMessage}</p>
                            <button className="absolute -bottom-2 -right-2 text-xl opacity-70">
                                <div className="flex">
                                    {
                                        message.MessageReactions?.filter((value, index, self) =>
                                            self.findIndex((t) => t.ReactionType === value.ReactionType) === index)?.map((reaction) => {
                                            return (
                                                <img key={Guid.create().toString()} src={reaction.ReactionUrl} alt="reaction" className="w-4 h-4"/>
                                            );
                                        })
                                    }
                                        </div>
                            </button>
                        </div>
                    </div>
                    <Tooltip title={<EmojiTooltip/>} trigger={"click"} color={"white"} overlayInnerStyle={{padding: "0px", borderRadius: "32px"}}  overlayStyle={{maxWidth: '500px'}} className="hidden group-hover:block" >
                        <button className="text-xl opacity-70 ml-4 relative">
                            <MdOutlineEmojiEmotions/>
                        </button>
                    </Tooltip>
                    <button className="hidden group-hover:block ml-4 opacity-70">
                        <IoArrowUndo />
                    </button>
                    <button className="hidden group-hover:block ml-4 opacity-70">
                        <BsThreeDots />
                    </button>

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
                <div className="w-full flex justify-end group">
                    <button className="mr-4 hidden group-hover:block opacity-70">
                        <BsThreeDots />
                    </button>
                    <button className="mr-4 hidden group-hover:block opacity-70">
                        <IoArrowRedoSharp/>
                    </button>

                    <Tooltip title={<EmojiTooltip/>} trigger={"click"} color={"white"} overlayInnerStyle={{
                        padding: "0px",
                        borderRadius: "32px"
                    }} overlayStyle={{ maxWidth: "500px" }} className="hidden group-hover:block">
                        <button className="text-xl opacity-70 mr-4">
                            <MdOutlineEmojiEmotions/>
                        </button>
                    </Tooltip>
                    <div>
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96 relative group">
                            <p className="text-sm break-words">{message.TextMessage}</p>
                            <button className="absolute -bottom-2 -left-2 text-xl opacity-70">
                                <div className="flex">
                                    {
                                        message.MessageReactions?.filter((value, index, self) =>
                                            self.findIndex((t) => t.ReactionType === value.ReactionType) === index)?.map((reaction) => {
                                            return (
                                                <img key={Guid.create().toString()} src={reaction.ReactionUrl} alt="reaction" className="w-4 h-4"/>
                                            );
                                        })
                                    }
                                </div>
                            </button>
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
                        </p> : null
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
