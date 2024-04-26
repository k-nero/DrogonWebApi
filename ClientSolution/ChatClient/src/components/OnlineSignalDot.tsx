import React from "react";

function OnlineSignalDot({ online }: { online: boolean }) {
    if (online)
    {
        return <div className="absolute bottom-0 right-0 w-3 h-3 bg-green-500 rounded-full border-2 border-white"/>;
    }
    else
    {
       return <div className="absolute bottom-0 right-0 w-3 h-3 bg-gray-500 rounded-full border-2 border-white"/>
    }
}

export default OnlineSignalDot;
