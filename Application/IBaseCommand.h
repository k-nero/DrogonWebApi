#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SQLAPI.h>
#include "ApplicationApi.h"

template<typename T>
class APPLICATION_API IBaseCommand
{
	virtual int Create(T* item) = 0;
	virtual int Update(T* item) = 0;
};