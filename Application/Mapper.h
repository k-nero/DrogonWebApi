#pragma once
#include <string>
#include <unordered_map>
#include <boost/describe.hpp>
#include <memory>
#include "ApplicationApi.h"

enum APPLICATION_API MemberOption
{
	Ignore,
	Override,
	OvverideIfNull
};

class APPLICATION_API Mapper
{
public:
	Mapper() =  default;
	~Mapper() = default;

	template<typename Source, typename Destination>
	static Destination Map(Source& source)
	{
		Destination destination{};
		boost::mp11::mp_for_each< boost::describe::describe_members<Destination, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto destination_memeber)
		{
			boost::mp11::mp_for_each< boost::describe::describe_members<Source, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto source_member)
			{

				void* source_pointer = &(source.*(source_member.pointer));
				using des_type = std::remove_reference_t<decltype(destination.*(destination_memeber.pointer))>;	
				if constexpr (source_member.name == destination_memeber.name && typeid(source.*(source_member.pointer)) == typeid(destination.*(destination_memeber.pointer)) )
				{
					destination.*(destination_memeber.pointer) = *((des_type*)source_pointer);
				}
			});
		});
		return destination;
	}

	template<typename Source, typename Destination>
	class MapProfile
	{
	public:
		 MapProfile() = default;
		 ~MapProfile() = default;

		using SourceType = Source;
		using DestinationType = Destination;

		template <typename Source, typename Destination>
		static MapProfile& CreateMap()
		{
			return new MapProfile<Source, Destination>();
		}

		MapProfile& ForMember( std::string memeber_name, MemberOption opt )
		{
			_memberOptions[memeber_name] = opt;
			return *this;
		}

		MapProfile& ReverseMap()
		{
			_reverseMap = true;
			return *this;
		}

		

	private:
		std::unordered_map<std::string, MemberOption> _memberOptions;
		bool _reverseMap = false;
	};

	
	
};
