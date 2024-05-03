import React from "react";

function FileOptions({ file, fileType }: { file: string, fileType?: string})
{
    return (  <>
            <div className="min-w-16" style={{ color: "black" }}>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button >
                        <a href={file} download>
                            <p>Download</p>
                        </a>
                    </button>
                </div>
                <div className="rounded-sm hover:bg-gray-300 p-1">
                    <button onClick={() => {
                        //download file to blob
                       navigator.clipboard.writeText(file).then(r => {});
                    }}>
                        <p>Copy url</p>
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
