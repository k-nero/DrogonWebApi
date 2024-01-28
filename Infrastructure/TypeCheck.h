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
