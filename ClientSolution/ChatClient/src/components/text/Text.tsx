import { Guid } from "guid-typescript";

function Text({ text }: { text: string })
{
    const words = text.split(" ");
    const regex = /(https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z]{2,}(\.[a-zA-Z]{2,})(\.[a-zA-Z]{2,})?\/[a-zA-Z0-9]{2,}|((https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z]{2,}(\.[a-zA-Z]{2,})(\.[a-zA-Z]{2,})?)|(https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z0-9]{2,}\.[a-zA-Z0-9]{2,}\.[a-zA-Z0-9]{2,}(\.[a-zA-Z0-9]{2,})?/g;

    return (
        <p >
            {
                words.map((word) => {
                    if (regex.test(word) ) {
                        return <a key={Guid.create().toString()} href={word} target="_blank" rel="noreferrer" className="underline text-blue-500">{word} </a>;
                    }
                    return word + " ";
                })
            }
    </p>);
}

export default Text;
