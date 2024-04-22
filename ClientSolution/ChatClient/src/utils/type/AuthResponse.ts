import ApplicationUser from "@/utils/type/ApplicationUser.ts";

export interface AuthResponse
{
    user: ApplicationUser;
    access_token: string;
}
