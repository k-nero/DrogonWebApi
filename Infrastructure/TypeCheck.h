#pragma once
#include <vector>
#include <list>
#include <memory>
#include <map>

template <typename C> struct is_vector : std::false_type {};
template <typename T, typename A> struct is_vector<std::vector<T, A>> : std::true_type {};
template <typename C> inline constexpr bool is_vector_v = is_vector<C>::value;

template <typename C> struct is_list : std::false_type {};
template <typename T, typename A> struct is_list<std::list<T, A>> : std::true_type {};
template <typename C> inline constexpr bool is_list_v = is_list<C>::value;

template <typename C> struct is_shared_ptr : std::false_type {};
template <typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
template <typename C> inline constexpr bool is_shared_ptr_v = is_shared_ptr<C>::value;

template <typename C> struct is_unique_ptr : std::false_type {};
template <typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
template <typename C> inline constexpr bool is_unique_ptr_v = is_unique_ptr<C>::value;

template <typename C> struct is_weak_ptr : std::false_type {};
template <typename T> struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
template <typename C> inline constexpr bool is_weak_ptr = is_weak_ptr<C>::value;

template <typename C> struct is_map : std::false_type {};
template <typename T, typename A> struct is_map<std::map<T, A>> : std::true_type {};
template <typename C> inline constexpr bool is_map_v = is_map<C>::value;

template <typename T> struct is_string : std::false_type {};
template <> struct is_string<std::string> : std::true_type {};
template <> struct is_string<std::wstring> : std::true_type {};
template <typename T> inline constexpr bool is_string_v = is_string<T>::value;

template <typename , typename = void> struct has_value_type
{
	using type = void;
};

template <typename T> struct has_value_type<T, std::void_t<typename T::value_type>>
{
	using type = T::value_type;
};

template <typename, typename = void> struct has_element_type
{
	using type = void;
};

template <typename T> struct has_element_type<T, std::void_t<typename T::element_type>>
{
	using type = T::element_type;
};

template <typename T> using has_value_type_t = typename has_value_type<T>::type;
template <typename T> using has_element_type_t = typename has_element_type<T>::type;

// Primary template
template <typename T, typename = void>
struct is_non_void : std::false_type {};

// Specialization for non-void types
template <typename T>
struct is_non_void<T, std::enable_if_t<!std::is_same<T, void>::value>> : std::true_type {};


inline bool is_primitive_type(auto val)
{
	return std::is_arithmetic_v<std::remove_reference_t<decltype(val)>> || std::is_same_v<std::remove_reference_t<decltype(val)>, std::string> || std::is_same_v<std::remove_reference_t<decltype(val)>, std::wstring>
		|| std::is_same_v<std::remove_reference_t<decltype(val)>, std::string_view> || std::is_same_v<std::remove_reference_t<decltype(val)>, std::wstring_view>;
}
