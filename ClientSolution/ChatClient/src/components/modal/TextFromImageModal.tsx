import { Button, Modal } from "antd";
import Tesseract from "tesseract.js";
import React from "react";
import { LuLoader2 } from "react-icons/lu";
import { CiTextAlignJustify } from "react-icons/ci";
import { Guid } from "guid-typescript";


function TextFromImageModal({ url }: { url: string })
{
    const [isRecognizing, setIsRecognizing] = React.useState(false);

    return (
        <button className="text-xl align-middle" onClick={() => {
            setIsRecognizing(true)
            Tesseract.recognize(url).then(r => {
                setIsRecognizing(false)
                Modal.info({
                    title: "Text from image",
                    content: <p>{r.data.text}</p>,
                    onOk: () => {},
                    onCancel: () => {},
                    centered: true,
                    closable: true,
                    maskClosable: true,
                    width: "50%",
                    footer: [
                        <div key={`${Guid.create().toString()}`} className="w-full text-end">
                            <Button key="text-ok" onClick={(e) => {
                                navigator.clipboard.writeText(r.data.text).then(r => {})
                            }} className="mt-4 ">Copy text</Button>,
                        </div>
                    ],
                });
            })
        }}>
            {
                isRecognizing ?
                    <>
                        <div className="animate-spin">
                            <LuLoader2/>
                        </div>
                    </>
                    : <CiTextAlignJustify />
            }
        </button>
    );
}

export default TextFromImageModal;
