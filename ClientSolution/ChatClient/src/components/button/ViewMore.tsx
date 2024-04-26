import React from "react";

function ViewMore({ func }: { func?: () => void })
{
    return(
            <div className="w-full mt-4 mx-4 text-center">
                <button className="bg-teal-500 w-96 h-7 rounded-xl" onClick={func}>
                    <p className="text-center text-white font-bold">View More</p>
                </button>
            </div>
        );
}

export default ViewMore;
