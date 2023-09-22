#include "pch.h"
#include "BcryptHash.h"

BcryptHash::BcryptHash()
{
    UCHAR salt[16] = { 0 };
    if (BCryptGenRandom(NULL, salt, 16, BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) 
    {
        // handle error
    }

    // Create a hash object and initialize it with bcrypt
    BCRYPT_ALG_HANDLE bcrypt;
    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&bcrypt, BCRYPT_SHA512_ALGORITHM, NULL, 0)))
    {
        // handle error
    }

    BCRYPT_HASH_HANDLE hash;
    ULONG cbResult;
    ULONG cbObject;
    if (!NT_SUCCESS(BCryptGetProperty(bcrypt, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbObject, sizeof(ULONG), &cbResult, 0)))
    {
        // handle error
    }

    PBYTE pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbObject);
    if (NULL == pbHashObject)
    {
        // handle error
    }

    if (!NT_SUCCESS(BCryptCreateHash(bcrypt, &hash, pbHashObject, cbObject, NULL, 0, 0)))
    {
        // handle error
    }

    // Hash the password using bcrypt and the random salt
    const char* password = "password123";
    DWORD cbPassword = strlen(password);
    if (!NT_SUCCESS(BCryptHashData(hash, (PBYTE)password, cbPassword, 0)))
    {
        // handle error
    }

    if (!NT_SUCCESS(BCryptHashData(hash, salt, sizeof(salt), 0)))
    {
        // handle error
    }

    // Finish the hash computation and obtain the buffer with the hash output
    PBYTE pbHash = NULL;
    ULONG cbHash = 0;
    if (!NT_SUCCESS(BCryptFinishHash(hash, pbHash, cbHash, 0)))
    {
        // handle error
    }

    pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
    if (NULL == pbHash)
    {
        // handle error
    }

    if (!NT_SUCCESS(BCryptFinishHash(hash, pbHash, cbHash, 0)))
    {
        // handle error
    }

    // Clean up resources
    BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(bcrypt, 0);
    HeapFree(GetProcessHeap(), 0, pbHash);
    HeapFree(GetProcessHeap(), 0, pbHashObject);

}

BcryptHash::~BcryptHash()
{
}

std::string BcryptHash::HashPassword(std::string password)
{
	return std::string();
}

bool BcryptHash::VerifyPassword(std::string password, std::string hash)
{
	return false;
}
