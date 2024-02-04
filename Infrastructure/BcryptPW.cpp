#include "pch.h"
#include "BcryptPW.h"

Bcrypt::Bcrypt()
{
}

Bcrypt::~Bcrypt()
{
}

std::string Bcrypt::HashPassword(const std::string& passwordStr) noexcept(false)
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

		BOOST_LOG_TRIVIAL(error) << "BCryptOpenAlgorithmProvider failed with status: " << std::hex << status;
		throw std::exception("BCryptOpenAlgorithmProvider failed");
		goto Cleanup;
	}

	status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		BOOST_LOG_TRIVIAL(error) << "BCryptGetProperty failed with status: " << std::hex << status;
		throw std::exception("BCryptGetProperty failed");
		goto Cleanup;
	}

	pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
	if (NULL == pbHashObject)
	{
		BOOST_LOG_TRIVIAL(error) << "Memory allocation failed";
		throw std::exception("Memory allocation failed");
		goto Cleanup;
	}

	status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, secret, secretSize, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		BOOST_LOG_TRIVIAL(error) << "BCryptCreateHash failed with status: " << std::hex << status;
		throw std::exception("BCryptCreateHash failed");
		goto Cleanup;
	}

	status = BCryptHashData(hHash, password, passwordSize, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		BOOST_LOG_TRIVIAL(error) << "BCryptHashData failed with status: " << std::hex << status;
		throw std::exception("BCryptHashData failed");
		goto Cleanup;
	}

	status = BCryptGetProperty(hHash, BCRYPT_HASH_LENGTH, (PBYTE)&hashSize, sizeof(DWORD), &cbData, 0);
	if (!(((NTSTATUS)(status)) >= 0))
	{
		
		BOOST_LOG_TRIVIAL(error) << "BCryptGetProperty failed with status: " << std::hex << status;
		throw std::exception("BCryptGetProperty failed");
		goto Cleanup;
	}

	hash = (PUCHAR)malloc(hashSize);
	if (hash == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "Memory allocation failed";
		throw std::exception("Memory allocation failed");
		goto Cleanup;
	}
	status = BCryptFinishHash(hHash, hash, hashSize, NULL);
	if (status == STATUS_INVALID_PARAMETER)
	{
		BOOST_LOG_TRIVIAL(fatal) << "The hashSize parameter is not valid. " << std::hex << status;
		throw std::exception("The hashSize parameter is not valid");
		goto Cleanup;
	}
	else if (status == STATUS_INVALID_HANDLE)
	{
		BOOST_LOG_TRIVIAL(fatal) << "The hHash parameter is not valid. " << std::hex << status;
		throw std::exception("The hHash parameter is not valid");
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


bool Bcrypt::VerifyPassword(std::string password, std::string hash) noexcept(false)
{
	/*int size = boost::beast::detail::base64::decoded_size(hash.size());
	char* decodedPassword = (char*)malloc(size);
	auto sizeStr = boost::beast::detail::base64::decode(decodedPassword, hash.c_str(), hash.size());*/
	try
	{
		std::string hashedPassword = HashPassword(password);
		if (hashedPassword == hash)
		{
			return true;
		}
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "Error: " << e.what();
		throw;
	}
	
	return false;
}
