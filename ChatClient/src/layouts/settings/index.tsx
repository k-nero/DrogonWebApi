import Navbar from "@/components/navbar";

function SettingsLayout({ children }: { children: React.ReactNode })
{
    return (
        <>
            <div className="grid grid-cols-12 w-full h-full">
                <Navbar/>
                {children}
            </div>
        </>
    );
}

export default SettingsLayout;
