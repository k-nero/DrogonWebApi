import MessageAttachType from "@/utils/type/MessageAttachType.ts";
import { Modal } from "antd";

function AudioModal({audio}: {audio: MessageAttachType})
{
    let attachName = audio.AttachName.split(".")[0];
    const attachExt = audio.AttachName.split(".")[1];

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
        content: <div className="w-full h-full text-center">
            <audio controls className="m-auto" style={{

            }}>
                <source src={audio.AttachUrl} type="audio/mpeg"/>
            </audio>
        </div>,
        footer: null,
        maskClosable: true,
        closable: true,
    });
}

export default AudioModal;
