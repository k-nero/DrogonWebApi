import MessageReactionType from "@/utils/type/MessageReactionType.ts";
import React, { useEffect, useState } from "react";
import ApplicationUser from "@/utils/type/ApplicationUser.ts";
import Query from "@/utils/function/Query.ts";

function UserReaction({ reaction }: { reaction: MessageReactionType[] })
{
    const [user, setUser] = useState<ApplicationUser>();
    useEffect(() =>
    {
        Query<ApplicationUser>(`/users/${reaction[0].ApplicationUserId}`).then((r) =>
        {
            setUser(r);
        });
    }, [reaction]);

    return (
        <div>
            <div className="flex justify-between">
                <div className="flex gap-2.5">
                    <img src={user?.AvatarUrl} alt={user?.UserName} className="w-6 h-6 rounded-full" />
                    <p>{user?.UserName}</p>
                </div>
                <p className={"text-gray-500"}>{
                    reaction.map((r) =>
                    {
                        return r.ReactionType;
                    })
                }</p>
            </div>
        </div>
    );
}

export default UserReaction;
