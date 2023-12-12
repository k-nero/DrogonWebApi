#pragma once

#ifdef INFRASTRUCTURE_EXPORTS
#define INFRASTRUCTURE_API __declspec(dllexport)
#else
#define INFRASTRUCTURE_API __declspec(dllimport)
#endif
