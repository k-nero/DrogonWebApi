import Message from "@/pages/chat/chat/components/Message.tsx";

function MessageBox({chat_id} : {chat_id: string})
{

    async function getMessages()
    {
        const response = await fetch(`http://localhost:8000/api/chat/${chat_id}/messages`, {
            method: "GET",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${localStorage.getItem("access_token")}`
            }
        });

        const data = await response.json();

        console.log(data);
    }

    return (
        <div className="w-full h-full p-6">
            <Message/>
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
            {/*<Message/>*/}
        </div>
    );
}

export default MessageBox;

