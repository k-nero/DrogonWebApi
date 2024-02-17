#pragma once
#include "BaseEntity.h"
#include <type_traits>
#include <functional>

/*template<typename T, typename Z = std::is_base_of<BaseEntity, T>::type >
class CommandBuilder
{

public:
	CommandBuilder();

	~CommandBuilder();
private:
	std::string _command;

}*/;

namespace CommandBuilder
{
	class Where
	{
	public:
		Where();
		~Where();
		


	private:
		std::string _command;
	};
}

