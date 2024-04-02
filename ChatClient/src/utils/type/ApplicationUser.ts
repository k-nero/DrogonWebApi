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
    ContactListId: string;
    chatParticipants: never[],
    ContactList: never,
}

export default ApplicationUser;