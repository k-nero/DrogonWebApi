import Markdown from "react-markdown";
//import SyntaxHighlighter from "react-syntax-highlighter";
import { Prism as SyntaxHighlighter } from "react-syntax-highlighter";
// import { dark } from 'react-syntax-highlighter/dist/esm/styles/prism';
// import { nightOwl } from "react-syntax-highlighter/dist/esm/styles/hljs";
// import { github } from "react-syntax-highlighter/dist/esm/styles/hljs";
import { oneDark } from "react-syntax-highlighter/dist/cjs/styles/prism";

function CodeView({ textMessage }: { textMessage?: string })
{
    return (
        <>
            <Markdown
                children={textMessage}
                components={{
                    code(props)
                    {
                        const { children, className, node, ...rest } = props;
                        const match = /language-(\w+)/.exec(className || "");
                        return (
                            <SyntaxHighlighter
                                showLineNumbers={true}
                                wrapLongLines={false}
                                wrapLines={false}
                                children={String(children).replace(/\n$/, "")}
                                language={match ? match[1] : "cpp"}
                                style={oneDark}
                                customStyle={{
                                    fontFamily: "ui-monospace !important",
                                    fontSize: "13px",
                                }}

                                codeTagProps={{
                                    style: {
                                        fontFamily: "ui-monospace !important",
                                        fontSize: "13px"
                                    }
                                }}
                            />
                        );
                    }
                }
                }
            />
        </>

    );
}

export default CodeView;
