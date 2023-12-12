#pragma once
#ifdef DOMAIN_EXPORTS
#define DOMAIN_API __declspec(dllexport)
#else
#define DOMAIN_API __declspec(dllimport)
#endif