import { Modal } from "antd";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import { FaMagnifyingGlassMinus, FaMagnifyingGlassPlus, FaPlus } from "react-icons/fa6";
import React from "react";
import { MdOutlineSaveAlt } from "react-icons/md";
import { FaRegShareSquare } from "react-icons/fa";

function ImageViewModal({ image, onClose }: { image: MessageAttachType, onClose?: () => void })
{
    let attachName = image.AttachName.split(".")[0];
    const attachExt = image.AttachName.split(".")[1];

    if(attachName.length > 20)
    {
        attachName = attachName.substring(0, 20) + "...";
    }

    return Modal.info({
        title: <div className="whitespace-nowrap">
            <p className="max-w-64 overflow-hidden ">
                {attachName + "." + attachExt}
            </p>
        </div>,
        icon: null,
        content: <div className="overflow-auto relative flex justify-center" style={{
            height: "80vh",
            width: "80vw",

        }}>
            <img loading="lazy" src={image.AttachUrl} alt={image.AttachName} className="" id={image.Id} style={{
                maxHeight: "80vh",
                maxWidth: "80vw",
                transformOrigin: "50% 50%",
            }}/>
        </div>,
        onOk: onClose ? onClose : () => {},
        onCancel: onClose ? onClose : () => {},
        centered: true,
        closable: true,
        maskClosable: true,
        width: "fit-content",
        styles: {

        },
        footer: <div className="flex justify-evenly gap-2.5 mt-4">
            <div className=" flex gap-4 py-2">
                <button className="text-xl align-middle">
                    <FaPlus/>
                </button>
            </div>
            <div className="flex gap-4 py-2">
                <button className="text-xl align-middle" onClick={() => {
                    const img = document.getElementById(image.Id) as HTMLImageElement;
                    const transform = img.style.transform || "scale(1)";
                    const scale = Number(transform.split("(")[1].split(")")[0]);
                    img.style.transform = `scale(${scale + 0.3})`;

                }}>
                    <FaMagnifyingGlassPlus/>
                </button>
                <button className="text-xl align-middle" onClick={() => {
                    const img = document.getElementById(image.Id) as HTMLImageElement;
                    const transform = img.style.transform || "scale(1)";
                    const scale = Number(transform.split("(")[1].split(")")[0]);
                    img.style.transform = `scale(${scale - 0.3})`;
                }}>
                    <FaMagnifyingGlassMinus/>
                </button>
            </div>
            <div className="flex gap-4 py-2">
                <button className=" text-xl align-middle" onClick={(e) => {
                    e.preventDefault();
                    window.open(image.AttachUrl, "_blank");
                }}><MdOutlineSaveAlt/>
                </button>
                <button className="text-xl align-middle">
                    <FaRegShareSquare/>
                </button>
            </div>

        </div>
    });
}

export default ImageViewModal;

