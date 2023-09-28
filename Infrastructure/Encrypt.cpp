#include "pch.h"
#include "Encrypt.h"

Encrypt::Encrypt()
{
}

Encrypt::~Encrypt()
{
}

std::string Encrypt::HashPassword(const std::string& password, const std::string& salt)
{
    const EVP_MD* md = EVP_sha512();
    const int iterations = 10000;
    std::string saltValue = salt;
    if (saltValue.empty())
    {
        unsigned char saltData[16];
        if (RAND_bytes(saltData, sizeof(saltData)) != 1)
        {

        }
        saltValue = std::string(reinterpret_cast<char*>(saltData), sizeof(saltData));
    }

    unsigned char hash[EVP_MAX_MD_SIZE]{};
    unsigned int hashLength = 0;

    for (int i = 0; i < iterations - 1; ++i)
    {
        if (EVP_Digest(hash, hashLength, hash, &hashLength, md, NULL) != 1)
        {

        }
    }

    std::string hashedPassword = std::string(reinterpret_cast<char*>(hash), hashLength);
    return hashedPassword;
}

bool Encrypt::VerifyPassword(std::string password, std::string hash)
{
    const EVP_MD* md = EVP_sha512();
    unsigned char computedHash[EVP_MAX_MD_SIZE]{};
    unsigned int computedHashLength = 0;

    for (int i = 0; i < 9999; ++i)
    {
        if (EVP_Digest(computedHash, computedHashLength, computedHash, &computedHashLength, md, NULL) != 1)
        {

        }
    }

    std::string computedHashedPassword = std::string(reinterpret_cast<char*>(computedHash), computedHashLength);
    return (computedHashedPassword == hash);
}
