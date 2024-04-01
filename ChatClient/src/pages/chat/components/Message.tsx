function Message()
{
    const incoming = true;
    if(incoming)
    {
        return (
            <div>
                <div className="w-full flex">
                    <div className="mt-auto">
                        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                    <div>
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96">
                            <p className="text-sm">Hello this is a test message</p>
                        </div>
                    </div>
                </div>
                <p className="text-xs text-gray-500 text-left ml-9 p-1">12:00 PM</p>
            </div>
        );
    }
    else
    {
        return (
            <div>
                <div className="w-full flex justify-end">
                    <div>
                        <div className="bg-white p-3 mx-3 rounded-xl max-w-96">
                            <p className="text-sm">Hello this is a test message</p>
                        </div>
                    </div>
                    <div className="mt-auto">
                        <img src="https://via.placeholder.com/150" alt="John Doe" className="w-6 h-6 rounded-full"/>
                    </div>
                </div>
                <p className="text-xs text-gray-500 text-right mr-9 p-1">12:00 PM</p>
            </div>
        );
    }
}

export default Message;
