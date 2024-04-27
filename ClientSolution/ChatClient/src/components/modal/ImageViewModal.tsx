import { Modal } from "antd";
import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import { FaMagnifyingGlassMinus, FaMagnifyingGlassPlus, FaPlus } from "react-icons/fa6";
import React from "react";
import { MdOutlineSaveAlt, MdOutlineTranslate } from "react-icons/md";
import { FaRegClipboard, FaRegShareSquare } from "react-icons/fa";
import TextFromImageModal from "@/components/modal/TextFromImageModal.tsx";
import { TiDocumentText } from "react-icons/ti";
import Tesseract from "tesseract.js";

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
        content:
            <>
                <div className="overflow-auto relative flex justify-center" style={{
                    height: "80vh",
                    width: "80vw"
                }}>
                    <img loading="lazy" src={image.AttachUrl} alt={image.AttachName} className="" id={image.Id} style={{
                        maxHeight: "80vh",
                        maxWidth: "80vw",
                        transformOrigin: "50% 50%"
                    }}/>
                </div>
            </>
        ,
        onOk: onClose ? onClose : () => {},
        onCancel: onClose ? onClose : () => {},
        centered: true,
        closable: true,
        maskClosable: true,
        width: "fit-content",
        styles: {},
        footer: <div className="flex justify-evenly gap-2.5 mt-4">
            <div className=" flex gap-4 py-2">
                <TextFromImageModal url={image.AttachUrl}/>
                <button className="text-xl align-middle" onClick={  (e) => {
                    const img = document.getElementById(image.Id) as HTMLImageElement;
                    img.crossOrigin = "anonymous";
                    const canvas = document.createElement("canvas").transferControlToOffscreen();
                    canvas.width = img.width;
                    canvas.height = img.height;
                    const ctx = canvas.getContext("2d");
                    img.onload = () => {
                        ctx?.drawImage(img, 0, 0, img.width, img.height);
                        canvas.convertToBlob({
                            type: "image/png",
                            quality: 1
                        }).then((blob) => {
                            if (!blob)
                            {
                                console.error("Blob is null");
                                return;
                            }
                            console.log(blob.size);
                            const item = new ClipboardItem({ "image/png" : blob });
                            navigator.clipboard.write([item]).then(() => {});
                        })
                    }
                } }>
                    <FaRegClipboard />
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

