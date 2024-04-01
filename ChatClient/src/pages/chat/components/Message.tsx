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
                        <div className="bg-gray-300 bg-opacity-30 p-3 mx-3 rounded-xl max-w-96">
                            <p className="text-sm">Hello this is a test message</p>
                        </div>
                    </div>
                </div>
                <p className="text-xs text-gray-500 text-right mr-4">12:00 PM</p>
            </div>
        );
    }
    else
    {
        return (
            <div className="w-full flex justify-end">
                <div>
                    <div className="bg-gray-300 bg-opacity-30 p-3 mx-3 rounded-xl max-w-96">
                        <p className="text-sm">Hello this is a test message</p>
                    </div>
                </div>
                <div className="mt-auto">
                    <img src="https://via.placeholder.com/150" alt="John Doe" className="w-6 h-6 rounded-full"/>
                </div>
            </div>
        );
    }

}

export default Message;
