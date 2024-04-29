import MessageSeenByType from "@/utils/type/MessageSeenByType.ts";
import React, { useEffect, useState } from "react";
import ApplicationUser from "@/utils/type/ApplicationUser.ts";
import Query from "@/utils/function/Query.ts";

function UserSeenBy({ seenBy }: { seenBy: MessageSeenByType })
{

    const [user, setUser] = useState<ApplicationUser>();
    useEffect(() =>
    {
        Query<ApplicationUser>(`/users/${seenBy.ApplicationUserId}`).then((r) =>
        {
            setUser(r);
        });
    }, [seenBy]);

    return (
        <div>
            <div className="flex justify-between">
                <div className="flex gap-2.5">
                    <img src={user?.AvatarUrl} alt={user?.UserName} className="w-6 h-6 rounded-full"/>
                    <p>{user?.UserName}</p>
                </div>
                <p className={"text-gray-500"}>{new Date(seenBy?.CreatedDate).toLocaleTimeString("en-US", {
                    hour: "2-digit",
                    minute: "2-digit"
                })}</p>
            </div>
        </div>
    );
}

export default UserSeenBy;
