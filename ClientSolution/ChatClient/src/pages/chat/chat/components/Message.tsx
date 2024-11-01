import MessageType from "@/utils/type/MessageType.ts";
import useLocalStorage from "@/utils/hooks/useLocalStorage.ts";
import { AuthResponse } from "@/utils/type/AuthResponse.ts";
import { IoArrowRedoSharp, IoArrowUndo, IoCheckmarkDoneOutline } from "react-icons/io5";
import { MdOutlineEmojiEmotions } from "react-icons/md";
import { Button, Modal, Tooltip } from "antd";
import React, { Dispatch, useEffect, useState } from "react";
import { EmojiClickData } from "emoji-picker-react";
import { BsThreeDots } from "react-icons/bs";
import Query from "@/utils/function/Query.ts";
import { uWebSockets } from "@/utils/WebSocket/WebSocket.ts";
import MessageReactionType from "@/utils/type/MessageReactionType.ts";
import Text from "@/components/text/Text.tsx";
import EmojiTooltip from "@/components/EmojiToolTip";
import translate from "translate";
import AttachView from "@/pages/chat/chat/components/message/AttachView.tsx";
import CodeView from "@/components/text/CodeMessage";
import UserSeenBy from "@/pages/chat/chat/components/message/UserSeenBy.tsx";
import UserReaction from "@/pages/chat/chat/components/message/UserReaction.tsx";
import QuoteMessage from "@/pages/chat/chat/components/message/QuoteMessage.tsx";

const baseUrl = new URL(`${import.meta.env.VITE_API_URL}`);

function Message({ message, setQuoteMessage }: {
    message: MessageType,
    setQuoteMessage: Dispatch<React.SetStateAction<MessageType | undefined>>
})
{
    const [userLocal] = useLocalStorage("auth_credential", {});
    const credential: AuthResponse = userLocal;
    const [translateText, setTranslateText] = useState<string>("");
    const incoming = message.ApplicationUserId !== credential.user.Id;

    const [shouldRerender, setShouldRerender] = useState<boolean>(false);

    useEffect(() => {
        uWebSockets.getInstance().addMessageSeenBySubscriber((event) => {
            const e_data = JSON.parse(event.data);
                    if (message.Id === e_data.message.MessageId)
                    {
                       setShouldRerender(prevState => !prevState);
                    }
                });
    }, []);

    useEffect(() => {
        uWebSockets.getInstance().addReactionSubscriber((event) => {
            const e_data = JSON.parse(event.data);

            if (message.Id === e_data.message.MessageId)
            {
                setShouldRerender(prevState => !prevState);
            }
        });
    }, []);

    const [isSeenByModalOpen, setIsSeenByModalOpen] = useState(false);
    const showSeenByModal = () =>
    {
        setIsSeenByModalOpen(true);
    };
    const handleSeenByOk = () =>
    {
        setIsSeenByModalOpen(false);
    };
    const handleSeenByCancel = () =>
    {
        setIsSeenByModalOpen(false);
    };

    const [isEmojiModalOpen, setEmojiModalOpen] = useState(false);
    const showEmojiModal = () =>
    {
        setEmojiModalOpen(true);
    };
    const handleEmojiOk = () =>
    {
        setEmojiModalOpen(false);
    };
    const handleEmojiCancel = () =>
    {
        setEmojiModalOpen(false);
    };

    const [isCodeModalOpen, setCodeModalOpen] = useState(false);
    const showCodeModal = () =>
    {
        setCodeModalOpen(true);
    };
    const handleCodeOk = () =>
    {
        setCodeModalOpen(false);
    };
    const handleCodeCancel = () =>
    {
        setCodeModalOpen(false);
    };


    function MessageOption()
    {

        return (
            <div className="min-w-16" style={{ color: "black" }}>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button onClick={() => {
                        setQuoteMessage(message);
                    }}>
                        <p>Reply</p>
                    </button>
                </div>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button onClick={() => {
                        navigator.clipboard.writeText(message.TextMessage).then(() => { });
                    }}>
                        <p>Copy</p>
                    </button>
                </div>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button>
                        <p>Recall</p>
                    </button>
                </div>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button>
                        <p>Forward</p>
                    </button>
                </div>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button>
                        Edit
                    </button>
                </div>
                {
                    !message.TextMessage?.startsWith("```") && message.TextMessage &&
                    <>
                        {
                            translateText === "" ?
                                <div className="rounded-sm hover:bg-gray-300 p-1">
                                    <button onClick={() => {
                                        translate(message.TextMessage, { to: "vi" }).then((r) => {
                                            setTranslateText(r);
                                        });
                                    }}>
                                        Translate
                                    </button>
                                </div>
                                :
                                <div className="rounded-sm hover:bg-gray-300 p-1">
                                    <button onClick={() => {
                                        setTranslateText("");
                                    }}>
                                        Original
                                    </button>
                                </div>
                        }</>
                }

                {
                    message.TextMessage?.startsWith("```") &&
                    <div className="rounded-sm hover:bg-gray-300 p-1">
                        <button onClick={showCodeModal}>
                            View Code
                        </button>
                    </div>
                }

            </div>
        );
    }

    function EmojiClickCallback(emoji: EmojiClickData, event: MouseEvent)
    {
        const reaction = message.MessageReactions?.find((r) => r.ApplicationUserId === credential.user.Id && r.ReactionType === emoji.emoji);
        if (!reaction)
        {
            fetch(`${baseUrl}/message-reaction`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    MessageId: message.Id,
                    ReactionType: emoji.emoji,
                    ApplicationUserId: credential.user.Id,
                    ReactionUrl: emoji.imageUrl,
                    ReactionCount: 1
                })
            }).then((r) =>
            {
                if (r.ok)
                {
                    r.json().then((rs) =>
                    {
                        Query<MessageReactionType>(`/message-reaction/${rs.id}`).then((mrs) =>
                        {
                            uWebSockets.getInstance().send(JSON.stringify({
                                type: "reaction",
                                channel: message.ChatRoomId,
                                message: mrs
                            }));
                        });
                    });
                }
            });
        }
        else
        {
            reaction.ReactionCount += 1;
            fetch(`${baseUrl}/message-reaction/${reaction.Id}`, {
                method: "PATCH",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(reaction)
            }).then(() => { });
        }
    }

    const modal = (
        <div>
            <Modal title="Message seen by" open={isSeenByModalOpen} onOk={handleSeenByOk} onCancel={handleSeenByCancel} footer={[]}>
                <div>
                    {
                        message.MessageSeenBys?.map((seenBy) => {
                            return (
                                <UserSeenBy key={seenBy.Id} seenBy={seenBy}/>
                            );
                        })
                    }
                </div>
            </Modal>

            <Modal title="Message reacted by" open={isEmojiModalOpen} onOk={handleEmojiOk} onCancel={handleEmojiCancel} footer={[]}>
                <div>
                    {
                        message.MessageReactions?.filter((value, index, self) =>
                            self.findIndex((t) => t.ApplicationUserId === value.ApplicationUserId) === index)?.map((reaction) => {
                            return reaction.ApplicationUserId;
                        })?.map((user_id) => {
                            const re = message.MessageReactions?.filter((value) => {
                                return value.ApplicationUserId === user_id;
                            });
                            if (!re)
                            {
                                return null;
                            }
                            return (
                                <UserReaction key={user_id} reaction={re}/>
                            );
                        })
                    }
                </div>
            </Modal>

            <Modal title="Code view" open={isCodeModalOpen} onOk={handleCodeOk} onCancel={handleCodeCancel} width="70%" footer={[
                <Button key="back" onClick={handleCodeCancel}>
                    Close
                </Button>,
                <Button key="edit" onClick={handleCodeOk}>
                    Edit
                </Button>
            ]}>
                <div style={{
                    maxHeight: "70vh",
                    overflow: "auto"
                }}>

                    <CodeView textMessage={message.TextMessage}/>
                </div>
            </Modal>
        </div>
    );

    if (incoming)
    {
        return (
            <div>
                {modal}
                <div className="w-full flex group ">
                    <div className="mt-auto">
                        <img loading="lazy" src={message?.ApplicationUser?.AvatarUrl} alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                    <div className="">

                        {
                            message.QuoteMessageId ? (
                                <div className=" max-w-96 w-fit">
                                    <QuoteMessage message={message} incoming={incoming}/>
                                </div>
                            ) : null
                        }

                        <div className="flex w-full">
                            <div className="bg-white p-3 mx-3 rounded-xl  max-w-96 relative">
                                {
                                    message.MessageAttachs?.length && message.MessageAttachs?.length > 0 ?
                                        <AttachView message={message}/>
                                        : null
                                }
                                <div className={`${ message.TextMessage && message.MessageAttachs?.length ? "mt-3" : "" }`}>
                                    <div className="overflow-auto text-sm break-words max-h-96">
                                        {
                                            message.TextMessage?.startsWith("```") ?
                                                <CodeView textMessage={message.TextMessage}/>
                                                : <>
                                                    {
                                                        translateText === "" ? <Text text={message.TextMessage}/> :
                                                            <Text text={translateText}/>
                                                    }
                                                </>
                                        }
                                    </div>
                                    <button className="absolute -bottom-2 -right-2 text-xl opacity-70" onClick={showEmojiModal}>
                                        <div className="flex">
                                            {
                                                message.MessageReactions?.filter((value, index, self) =>
                                                    self.findIndex((t) => t.ReactionType === value.ReactionType) === index)?.map((reaction) => {
                                                    return (
                                                        <img loading="lazy" key={reaction.Id} src={reaction.ReactionUrl} alt="reaction" className="w-4 h-4"/>
                                                    );
                                                })
                                            }
                                        </div>
                                    </button>
                                </div>

                            </div>
                            <Tooltip title={<EmojiTooltip EmojiClickCallback={EmojiClickCallback}/>} trigger={"click"} color={"white"} overlayInnerStyle={{
                                padding: "0px",
                                borderRadius: "32px"
                            }} overlayStyle={{ maxWidth: "500px" }} className="hidden group-hover:block">
                                <button className="text-xl opacity-70 ml-4 relative" title="React message">
                                    <MdOutlineEmojiEmotions/>
                                </button>
                            </Tooltip>
                            <button className="hidden group-hover:block ml-4 opacity-70" title="Reply message">
                                <IoArrowUndo onClick={() => {
                                    setQuoteMessage(message);
                                }}/>
                            </button>
                            <Tooltip title={<MessageOption/>} trigger={"click"} color={"white"}
                                //overlayInnerStyle={{ padding: "0px", borderRadius: "32px" }}
                                //overlayStyle={{ maxWidth: '500px' }}
                                     placement={"right"}
                                     className="hidden group-hover:block">
                            <button className="ml-4 opacity-70" title="Message options">
                                    <BsThreeDots />
                                </button>
                            </Tooltip>
                        </div>
                    </div>


                </div>
                <div className="flex gap-2.5 mt-2">
                    {
                            <p className="text-xs text-gray-500 text-left ml-9 p-1">{
                                new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                                    hour: "2-digit",
                                    minute: "2-digit"
                                })
                            }</p>
                    }
                    {
                        message.MessageSeenBys?.length && message.MessageSeenBys?.length > 0 ?
                            <button onClick={showSeenByModal}>
                                <p className="text-md text-gray-700 text-left p-1">
                                    <IoCheckmarkDoneOutline />
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
                {modal}
                <div className="w-full flex justify-end group">

                    <div className="">
                        {
                            message.QuoteMessageId ? (
                                <div className="ml-auto max-w-96 w-fit">
                                    <QuoteMessage message={message} incoming={incoming}/>
                                </div>
                            ) : null
                        }

                        <div className="w-fit ml-auto flex">
                            <Tooltip title={<MessageOption/>} trigger={"click"} color={"white"}
                                //overlayInnerStyle={{ padding: "0px", borderRadius: "32px" }}
                                //overlayStyle={{ maxWidth: '500px' }}
                                     placement={"left"}
                                     className="hidden group-hover:block">
                                <button className="mr-4 opacity-70" title="Message options">
                                    <BsThreeDots/>
                                </button>
                            </Tooltip>
                            <button className="mr-4 hidden group-hover:block opacity-70" title="Reply message">
                                <IoArrowRedoSharp onClick={() => {
                                    setQuoteMessage(message);
                                }}/>
                            </button>

                            <Tooltip title={<EmojiTooltip EmojiClickCallback={EmojiClickCallback}/>} trigger={"click"} color={"white"} overlayInnerStyle={{
                                padding: "0px",
                                borderRadius: "32px"
                            }} overlayStyle={{ maxWidth: "500px" }} className="hidden group-hover:block">
                                <button className="text-xl opacity-70 mr-4" title="React message">
                                    <MdOutlineEmojiEmotions/>
                                </button>
                            </Tooltip>
                            <div className="bg-white p-3 mx-3 rounded-xl max-w-96 relative ">
                                {
                                    message.MessageAttachs?.length && message.MessageAttachs?.length > 0 ?

                                        <AttachView message={message} />
                                        : null
                                }
                                <div className={`${ message.TextMessage && message.MessageAttachs?.length ? "mt-3" : "" } ml-auto` }>
                                    <div className="text-sm break-words overflow-auto max-h-96 ">
                                        {
                                            message.TextMessage?.startsWith("```") ?
                                                <CodeView textMessage={message.TextMessage}/>
                                                : <>
                                                    {
                                                        translateText === "" ? <Text text={message.TextMessage}/> : <Text text={translateText}/>
                                                    }
                                                </>
                                        }
                                    </div>
                                    <button className="absolute -bottom-2 -left-2 text-xl opacity-70" onClick={showEmojiModal}>
                                        <div className="flex">
                                            {
                                                message.MessageReactions?.filter((value, index, self) =>
                                                    self.findIndex((t) => t.ReactionType === value.ReactionType) === index)?.map((reaction) => {
                                                    return (
                                                        <img loading="lazy" key={reaction.Id} src={reaction.ReactionUrl} alt="reaction" className="w-4 h-4"/>
                                                    );
                                                })
                                            }
                                        </div>
                                    </button>
                                </div>
                            </div>
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
                            <p className="text-xs text-gray-500 text-right mr-9 p-1">{
                                new Date(message.CreatedDate).toLocaleTimeString("en-US", {
                                    hour: "2-digit",
                                    minute: "2-digit"
                                })
                            }</p>
                    }
                </div>
            </div>
        );
    }
}

const MemoizedMessage = React.memo(Message);

export { MemoizedMessage };

export default Message;
