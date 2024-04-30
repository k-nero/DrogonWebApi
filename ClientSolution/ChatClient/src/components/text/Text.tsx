import Markdown from "react-markdown";
import remarkMath from "remark-math";
import rehypeRaw from "rehype-raw";
import rehypeMathjax, {Options} from "rehype-mathjax/svg";

function Text({ text }: { text: string })
{
    const words = text.split(" ");
    const regex = /(https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z]{2,}(\.[a-zA-Z]{2,})(\.[a-zA-Z]{2,})?\/[a-zA-Z0-9]{2,}|((https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z]{2,}(\.[a-zA-Z]{2,})(\.[a-zA-Z]{2,})?)|(https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z0-9]{2,}\.[a-zA-Z0-9]{2,}\.[a-zA-Z0-9]{2,}(\.[a-zA-Z0-9]{2,})?/g;

    // return (
    //     <p >
    //         {
    //             words.map((word) => {
    //                 if (regex.test(word) ) {
    //                     return <a key={Guid.create().toString()} href={word} target="_blank" rel="noreferrer" className="underline text-blue-500">{word} </a>;
    //                 }
    //                 return word + " ";
    //             })
    //         }
    // </p>);
    let message: string = "";

    words.forEach((word) => {
        if (regex.test(word))
        {
            message += `<a href="${word}" target="_blank" rel="noreferrer" class="underline text-blue-500">${word}</a> `;
        }
        else
        {
            message += word + " ";
        }
    });

    const options: Options = {
        svg:{
            scale: 1.3
        }
    }

    return (
        <div style={{
            maxHeight: "100% !important"
        }}>
            <Markdown remarkPlugins={[remarkMath]} rehypePlugins={[[
                rehypeMathjax, options
            ], rehypeRaw]} remarkRehypeOptions={{}}>
                {message}
            </Markdown>
        </div>

    );
}

export default Text;
