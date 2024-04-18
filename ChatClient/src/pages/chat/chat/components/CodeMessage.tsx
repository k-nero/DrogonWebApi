import Markdown from "react-markdown";
import SyntaxHighlighter from "react-syntax-highlighter";
import { nightOwl } from "react-syntax-highlighter/dist/esm/styles/hljs";
import React from "react";

function CodeView({textMessage}: { textMessage?: string})
{
    return (
        <pre >
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
                                    style={nightOwl}
                                />
                            );
                        }
                    }
                    }
                />
            </pre>
    );
}

export default CodeView;
