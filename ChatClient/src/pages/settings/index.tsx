import SettingsLayout from "@/layouts/settings";
import { Outlet } from "react-router-dom";

function Settings()
{
    return (
      <SettingsLayout>
        <Outlet/>
      </SettingsLayout>
    );
}

export default Settings;
