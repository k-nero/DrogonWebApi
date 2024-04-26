import React from "react";

function FileOptions()
{
    return (  <>
            <div className="min-w-16" style={{ color: "black" }}>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button onClick={() => {
                    }}>
                        <p>Download</p>
                    </button>
                </div>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button onClick={() => {
                    }}>
                        <p>Copy</p>
                    </button>
                </div>

                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button>
                        <p>Share</p>
                    </button>
                </div>
            </div>
        </>
    );
}

export default FileOptions;
