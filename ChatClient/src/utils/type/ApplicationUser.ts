interface ApplicationUser
{
    Id: string;
    UserName: string;
    PasswordHash: string;
    Email: string;
    PhoneNumber?: string;
    AvatarUrl?: string;
    CreatedDate: Date;
    ModifiedDate?: Date;
    ContactId: string;
}

export default ApplicationUser;
