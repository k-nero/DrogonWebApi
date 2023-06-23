#include "pch.h"
#include "ApplicationUser.h"

ApplicationUser::ApplicationUser() = default;

ApplicationUser::ApplicationUser( const ApplicationUser& applicationuser )
{
	Id = applicationuser.Id;
	UserName = applicationuser.UserName;
	NormalizedUserName = applicationuser.NormalizedUserName;
	Email = applicationuser.Email;
	EmailConfirmed = applicationuser.EmailConfirmed;
	NormalizedEmail = applicationuser.NormalizedEmail;
	PasswordHash = applicationuser.PasswordHash;
	SecurityStamp = applicationuser.SecurityStamp;
	ConcurrencyStamp = applicationuser.ConcurrencyStamp;
	PhoneNumber = applicationuser.PhoneNumber;
	PhoneNumberConfirmed = applicationuser.PhoneNumberConfirmed;
	TwoFactorEnabled = applicationuser.TwoFactorEnabled;
	LockoutEnd = applicationuser.LockoutEnd;
	LockoutEnabled = applicationuser.LockoutEnabled;
	AccessFailedCount = applicationuser.AccessFailedCount;
}

ApplicationUser::ApplicationUser( ApplicationUser&& applicationuser ) noexcept
{
	Id = std::move( applicationuser.Id );
	UserName = std::move( applicationuser.UserName );
	NormalizedUserName = std::move( applicationuser.NormalizedUserName );
	Email = std::move( applicationuser.Email );
	EmailConfirmed = applicationuser.EmailConfirmed;
	NormalizedEmail = std::move( applicationuser.NormalizedEmail );
	PasswordHash = std::move( applicationuser.PasswordHash );
	SecurityStamp = std::move( applicationuser.SecurityStamp );
	ConcurrencyStamp = std::move( applicationuser.ConcurrencyStamp );
	PhoneNumber = std::move( applicationuser.PhoneNumber );
	PhoneNumberConfirmed = applicationuser.PhoneNumberConfirmed;
	TwoFactorEnabled = applicationuser.TwoFactorEnabled;
	LockoutEnd = std::move( applicationuser.LockoutEnd );
	LockoutEnabled = applicationuser.LockoutEnabled;
	AccessFailedCount = applicationuser.AccessFailedCount;
}

ApplicationUser& ApplicationUser::operator=( const ApplicationUser& applicationuser )
= default;


/*ApplicationUser& ApplicationUser::operator=( ApplicationUser&& applicationuser ) noexcept
{
}*/

ApplicationUser::ApplicationUser( const std::string& id, const std::string& username, const std::string& normalizedusername, const std::string& email, const bool& emailconfirmed, const std::string& normalizedemail, const std::string& passwordhash, const std::string& securitystamp, const std::string& concurrencystamp, const std::string& phonenumber, const bool& phonenumberconfirmed, const bool& twofactorenabled, const std::string& lockoutend, const bool& lockoutenabled, const int& accessfailedcount )
{
	Id = id;
	UserName = username;
	NormalizedUserName = normalizedusername;
	Email = email;
	EmailConfirmed = emailconfirmed;
	NormalizedEmail = normalizedemail;
	PasswordHash = passwordhash;
	SecurityStamp = securitystamp;
	ConcurrencyStamp = concurrencystamp;
	PhoneNumber = phonenumber;
	PhoneNumberConfirmed = phonenumberconfirmed;
	TwoFactorEnabled = twofactorenabled;
	LockoutEnd = lockoutend;
	LockoutEnabled = lockoutenabled;
	AccessFailedCount = accessfailedcount;
}

std::string ApplicationUser::ToString()
{
	return "Id: " + Id + "\n" +
		"UserName: " + UserName + "\n" +
		"NormalizedUserName: " + NormalizedUserName + "\n" +
		"Email: " + Email + "\n" +
		"EmailConfirmed: " + std::to_string( EmailConfirmed ) + "\n" +
		"NormalizedEmail: " + NormalizedEmail + "\n" +
		"PasswordHash: " + PasswordHash + "\n" +
		"SecurityStamp: " + SecurityStamp + "\n" +
		"ConcurrencyStamp: " + ConcurrencyStamp + "\n" +
		"PhoneNumber: " + PhoneNumber + "\n" +
		"PhoneNumberConfirmed: " + std::to_string( PhoneNumberConfirmed ) + "\n" +
		"TwoFactorEnabled: " + std::to_string( TwoFactorEnabled ) + "\n" +
		"LockoutEnd: " + LockoutEnd + "\n" +
		"LockoutEnabled: " + std::to_string( LockoutEnabled ) + "\n" +
		"AccessFailedCount: " + std::to_string( AccessFailedCount ) + "\n";
}

ApplicationUser::~ApplicationUser() = default;