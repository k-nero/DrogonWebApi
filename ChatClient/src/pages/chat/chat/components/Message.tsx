import MessageType from "@/utils/type/MessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { IoArrowRedoSharp, IoArrowUndo, IoCheckmarkDoneOutline } from "react-icons/io5";
import { MdOutlineEmojiEmotions } from "react-icons/md";
import { Button, Modal, Tooltip } from "antd";
import React, { ReactNode, useEffect, useState } from "react";
import EmojiPicker, { EmojiClickData, EmojiStyle } from "emoji-picker-react";
import { BsThreeDots } from "react-icons/bs";
import Query from "@/utils/function/Query.ts";
import { Guid } from "guid-typescript";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import MessageReactionType from "@/utils/type/MessageReactionType.ts";
import ApplicationUser from "@/utils/type/ApplicationUser.ts";
import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";
const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function Message({message, showTime = true } : {message: MessageType, showTime: boolean})
{
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;
    //const [mess, setMessage] = useState<MessageType>(message);
    const incoming = message.ApplicationUserId !== credential.user.Id;

    const [isSeenByModalOpen, setIsSeenByModalOpen] = useState(false);
    const showSeenByModal = () => {
        setIsSeenByModalOpen(true);
    };
    const handleSeenByOk = () => {
        setIsSeenByModalOpen(false);
    };
    const handleSeenByCancel = () => {
        setIsSeenByModalOpen(false);
    };

    const [isEmojiModalOpen, setEmojiModalOpen] = useState(false);
    const showEmojiModal = () => {
        setEmojiModalOpen(true);
    };
    const handleEmojiOk = () => {
        setEmojiModalOpen(false);
    };
    const handleEmojiCancel = () => {
        setEmojiModalOpen(false);
    };
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
            <EmojiPicker  onReactionClick={(emoji, event) => EmojiClickCallback(emoji, event) }  reactionsDefaultOpen={true} emojiStyle={EmojiStyle.GOOGLE} />
        );
    }

    function UserSeenBy({seenBy} : {seenBy: MessageSeenByType})
    {

        const [user, setUser] = useState<ApplicationUser>();
        useEffect(() => {
            Query<ApplicationUser>(`/users/${seenBy.ApplicationUserId}`).then((r) => {
                setUser(r);
            });
        }, [seenBy]);

        return (
            <div>
                <div className="flex justify-between">
                    <div className="flex gap-2.5">
                        <img src={user?.AvatarUrl} alt={user?.UserName} className="w-6 h-6 rounded-full"/>
                        <p>{user?.UserName}</p>
                    </div>
                    <p className={"text-gray-500"}>{new Date(seenBy?.CreatedDate).toLocaleTimeString("en-US", {
                        hour: "2-digit",
                        minute: "2-digit"
                    })}</p>
                </div>
            </div>
        );
    }

    function UserReaction({reaction} : {reaction: MessageReactionType})
    {

        const [user, setUser] = useState<ApplicationUser>();
        useEffect(() => {
            Query<ApplicationUser>(`/users/${reaction.ApplicationUserId}`).then((r) => {
                setUser(r);
            });
        }, [reaction]);

        return (
            <div>
                <div className="flex justify-between">
                    <div className="flex gap-2.5">
                        <img src={user?.AvatarUrl} alt={user?.UserName} className="w-6 h-6 rounded-full"/>
                        <p>{user?.UserName}</p>
                    </div>
                    <p className={"text-gray-500"}>{new Date(reaction?.CreatedDate).toLocaleTimeString("en-US", {
                        hour: "2-digit",
                        minute: "2-digit"
                    })}</p>
                </div>
            </div>
        );
    }

    if (incoming)
    {
        return (
            <div>
                <Modal title="Message seen by" open={isSeenByModalOpen} onOk={handleSeenByOk} onCancel={handleSeenByCancel}  footer={[]} >
                   <div>
                          {
                            message.MessageSeenBys?.map((seenBy) => {
                                return (
                                    <UserSeenBy key={Guid.create().toString()} seenBy={seenBy}/>
                                );
                            })
                          }
                   </div>
                </Modal>

                <Modal title="Message reacted by" open={isEmojiModalOpen} onOk={handleEmojiOk} onCancel={handleEmojiCancel}  footer={[]} >
                    <div>
                        {
                            message.MessageReactions?.map((reaction) => {
                                return (
                                   <UserReaction key={Guid.create().toString()} reaction={reaction}/>
                                );
                            })
                        }
                    </div>
                </Modal>
                <div className="w-full flex group " >
                    <div className="mt-auto">
                        <img src={message?.ApplicationUser?.AvatarUrl} alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                    <div className="group">
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96 relative">
                            <p className="text-sm break-words">{message.TextMessage}</p>
                            <button className="absolute -bottom-2 -right-2 text-xl opacity-70" onClick={showEmojiModal}>
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
                <div className="flex gap-2.5 mt-2">
                    {
                        showTime ?
                            <p className="text-xs text-gray-500 text-left ml-9 p-1">{
                                new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                                    hour: "2-digit",
                                    minute: "2-digit"
                                })
                            }</p>
                            : null
                    }
                    {
                        message.MessageSeenBys?.length && message.MessageSeenBys?.length > 0 ?
                            <button onClick={showSeenByModal}>
                                <p className="text-md text-gray-700 text-left p-1">
                                    <IoCheckmarkDoneOutline/>
                                </p>
                            </button>
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
                <Modal title="Message seen by" open={isSeenByModalOpen} onOk={handleSeenByOk} onCancel={handleSeenByCancel}  footer={[]} >
                    <div>
                        {
                            message.MessageSeenBys?.map((seenBy) => {
                                return (
                                    <UserSeenBy key={Guid.create().toString()} seenBy={seenBy}/>
                                );
                            })
                        }
                    </div>
                </Modal>
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
                <div className="flex justify-end gap-2.5 mt-2">
                    {
                        message.MessageSeenBys?.length && message.MessageSeenBys?.length > 0 ?
                            <button onClick={showSeenByModal}>
                                <p className="text-md text-gray-700 text-left p-1">
                                    <IoCheckmarkDoneOutline/>
                                </p>
                            </button>
                            : null
                    }
                    {
                        showTime ?
                            <p className="text-xs text-gray-500 text-right mr-9 p-1">{
                                new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                                    hour: "2-digit",
                                    minute: "2-digit"
                                })
                            }</p>
                            : null
                    }
                </div>
            </div>
        );
    }
}

export default Message;
