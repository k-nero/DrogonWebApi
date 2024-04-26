import EmojiPicker, { EmojiClickData, EmojiStyle } from "emoji-picker-react";
import React from "react";

function EmojiTooltip({ EmojiClickCallback, reactionOpen = true}: { EmojiClickCallback: (emoji: EmojiClickData, event: any) => void, reactionOpen?: boolean})
{
    return (
        <EmojiPicker onReactionClick={(emoji, event) => EmojiClickCallback(emoji, event)}
                     onEmojiClick={(emoji, event) => EmojiClickCallback(emoji, event)}
                     reactionsDefaultOpen={reactionOpen} emojiStyle={EmojiStyle.GOOGLE} lazyLoadEmojis={true}  />
    );
}

const EmojiTooltipMemo = React.memo(EmojiTooltip);
export { EmojiTooltipMemo };

export default EmojiTooltip;
