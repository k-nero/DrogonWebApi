import MessageType from "@/utils/type/MessageType.ts";
import ImageViewModal from "@/components/modal/ImageViewModal.tsx";
import React from "react";

function AttachView({message}: {message: MessageType})
{
    return (<div className="flex max-w-96 gap-2.5 ">
        {
            message.MessageAttachs &&
            message.MessageAttachs.map((attach) => {
                const ext = attach.AttachName.split(".")[attach.AttachName.split(".").length - 1];
                if (attach.AttachType === "image")
                {
                    return (
                        <div key={attach.Id} className="w-fit max-h-64">
                            <button className="my-auto" onClick={() => {
                                ImageViewModal({ image: attach });
                            }}>
                                <img loading="lazy" src={attach.AttachUrl} alt={attach.AttachName} className="max-h-64 object-cover m-auto rounded-md"/>

                            </button>
                        </div>
                    );
                }
                else if (attach.AttachType === "video")
                {
                    return (
                        <div key={attach.Id} className="w-fit h-fit justify-end ">
                            <a href={attach.AttachUrl} className="text-sm text-gray-500 underline" target="_blank">{attach.AttachName}</a>
                            <video src={attach.AttachUrl} controls className="mt-1 h-32 m-auto"/>
                        </div>
                    );
                }
                else if (attach.AttachType === "audio")
                {
                    return (
                        <div key={attach.Id} className="w-fit h-fit justify-start ">
                            <a href={attach.AttachUrl} className="text-sm text-gray-500 underline" target="_blank">{attach.AttachName}</a>
                            <audio src={attach.AttachUrl} controls className="mt-1 m-auto"/>
                        </div>
                    );
                }
                else if (attach.AttachType === "text")
                {
                    return (
                        <div key={attach.Id} className="w-fit h-fit justify-start ">
                            <a href={attach.AttachUrl} className="text-sm text-gray-500 underline" target="_blank">{attach.AttachName}</a>
                            <img src={`/src/assets/text/${ext}.png`} className="mt-1 h-24 m-auto" alt={attach.AttachName}/>
                        </div>
                    );
                }
                else
                {
                    return (
                        <div key={attach.Id} className="w-fit justify-end  ">
                            <a href={attach.AttachUrl} target="_blank" rel="noreferrer" className="text-blue-500 underline">{attach.AttachName}</a>
                        </div>
                    );
                }

            })
        }
    </div>);
}

export default AttachView;
