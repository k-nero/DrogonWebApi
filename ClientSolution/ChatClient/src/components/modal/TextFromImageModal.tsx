import { Button, Modal, Tooltip } from "antd";
import { createWorker, OEM } from "tesseract.js";
import React from "react";
import { LuLoader2 } from "react-icons/lu";
import { CiTextAlignJustify } from "react-icons/ci";
import { Guid } from "guid-typescript";


function TextFromImageModal({ url }: { url: string })
{
    const [isRecognizing, setIsRecognizing] = React.useState(false);

    function ConfidenceColor(confidence: number)
    {
        if(confidence > 75)
        {
            return "text-green-500";
        }
        else if(confidence > 50)
        {
            return "text-yellow-500";
        }
        else
        {
            return "text-red-500";
        }
    }

    return (
        <button className="text-xl align-middle" onClick={async () => {
            setIsRecognizing(true);
            // @ts-ignore
            const worker = await createWorker(["chi_sim", "eng"], OEM.DEFAULT, {
                langPath: "https://tessdata.projectnaptha.com/4.0.0_best"
            });
            const { data } = await worker.recognize(url);
            Modal.info({
                title: "Text from image",
                content: <>
                    <p>{data.text}</p>
                </>,
                onOk: () => {},
                onCancel: () => {},
                centered: true,
                closable: true,
                maskClosable: true,
                width: "50%",
                footer: [
                    <div key={`${Guid.create().toString()}`} className="w-full flex justify-between mt-4">
                        <Tooltip title="Confidence point is the accuracy of the text recognition">
                            <div className="font-bold">
                                Confidence point: <p className={`${ ConfidenceColor(data.confidence) } inline-block`}>{data.confidence}</p>
                            </div>
                        </Tooltip>
                        <div>
                            <Button key="text-ok" onClick={(e) => {
                                navigator.clipboard.writeText(data.text).then(r => {});
                            }} className="">Copy text</Button>
                        </div>
                    </div>
                ]
            });
            await worker.terminate();
            setIsRecognizing(false);


        }}>
            {
                isRecognizing ?
                    <>
                        <div className="animate-spin">
                            <LuLoader2/>
                        </div>
                    </>
                    : <CiTextAlignJustify/>
            }
        </button>
    );
}

export default TextFromImageModal;