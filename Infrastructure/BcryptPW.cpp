#include "pch.h"
#include "BcryptPW.h"

Bcrypt::Bcrypt()
{
}

Bcrypt::~Bcrypt()
{
}

std::string Bcrypt::HashPassword(const std::string& passwordStr)
{
	if (ConfigProvider::GetInstance()->GetBcryptSecret().empty())
	{
		ConfigProvider::GetInstance()->Initialize();
	}
	auto sec = ConfigProvider::GetInstance()->GetBcryptSecret();

	PUCHAR secret = (PUCHAR)sec.data();
	ULONG secretSize = sec.size();

	BCRYPT_ALG_HANDLE hAlg = NULL;
	BCRYPT_HASH_HANDLE hHash = NULL;
	NTSTATUS status = 0;
	DWORD cbData = 0, cbHash = 0, cbHashObject = 0;
	PBYTE pbHashObject = NULL;
	DWORD passwordSize = passwordStr.size();

	ULONG hashSize = 0;
	PUCHAR hash = nullptr;

	PUCHAR password = (PUCHAR)malloc(passwordSize * sizeof(UCHAR));
	memcpy(password, passwordStr.c_str(), passwordSize * sizeof(UCHAR));

	status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA512_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		std::cerr << "BCryptOpenAlgorithmProvider failed with status: " << std::hex << status << std::endl;
		goto Cleanup;
	}

	status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		std::cerr << "BCryptGetProperty failed with status: " << std::hex << status << std::endl;
		goto Cleanup;
	}

	pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
	if (NULL == pbHashObject)
	{
		std::cerr << "Memory allocation failed" << std::endl;
		goto Cleanup;
	}

	status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, secret, secretSize, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		std::cerr << "BCryptCreateHash failed with status: " << std::hex << status << std::endl;
		goto Cleanup;
	}

	status = BCryptHashData(hHash, password, passwordSize, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		std::cerr << "BCryptHashData failed with status: " << std::hex << status << std::endl;
		goto Cleanup;
	}

	status = BCryptGetProperty(hHash, BCRYPT_HASH_LENGTH, (PBYTE)&hashSize, sizeof(DWORD), &cbData, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		std::cerr << "BCryptGetProperty failed with status: " << std::hex << status << std::endl;
		goto Cleanup;
	}

	hash = (PUCHAR)malloc(hashSize);
	if (hash == nullptr)
	{
		std::cerr << "Memory allocation failed" << std::endl;
		goto Cleanup;
	}
	status = BCryptFinishHash(hHash, hash, hashSize, NULL);
	if (status == STATUS_INVALID_PARAMETER)
	{
		std::cerr << "One or more parameters are not valid. This includes the case where cbOutput is not the same size as the hash. " << std::hex << status << std::endl;
		goto Cleanup;
	}
	else if (status == STATUS_INVALID_HANDLE)
	{
		std::cerr << "The hash handle in the hHash parameter is not valid. " << std::hex << status << std::endl;
		goto Cleanup;
	}


Cleanup:
	if (hAlg)
	{
		BCryptCloseAlgorithmProvider(hAlg, 0);
	}
	if (hHash)
	{
		BCryptDestroyHash(hHash);
	}
	if (pbHashObject)
	{
		HeapFree(GetProcessHeap(), 0, pbHashObject);
	}
	if (password)
	{
		free(password);
	}
	
	if (status >= 0 && hash != nullptr)
	{
		int size = boost::beast::detail::base64::encoded_size(hashSize);
		char* hashStr = (char*)malloc(size * sizeof(char));
		int hashedPasswordSize = boost::beast::detail::base64::encode(hashStr, hash, hashSize);
		if (hash)
		{
			free(hash);
		}
		if (hashStr != nullptr)
		{
			std::string hashedPassword = "";
			for (int i = 0; i < hashedPasswordSize; i++)
			{
				hashedPassword += hashStr[i];
			}
			free(hashStr);
			return hashedPassword;
		}
	}
	return "";
}


bool Bcrypt::VerifyPassword(std::string password, std::string hash)
{
	/*int size = boost::beast::detail::base64::decoded_size(hash.size());
	char* decodedPassword = (char*)malloc(size);
	auto sizeStr = boost::beast::detail::base64::decode(decodedPassword, hash.c_str(), hash.size());*/
	std::string hashedPassword = HashPassword(password);
	if (hashedPassword == hash)
	{
		return true;
	}
	return false;
}
