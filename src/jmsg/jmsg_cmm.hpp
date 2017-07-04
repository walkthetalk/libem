#pragma once

#include <vector>
#include <array>
#include <string>

#include "rapidjson/document.h"

/**
 * \brief helper class
 */
template <typename T> struct is_std_vector : std::false_type {};
template <typename T>
struct is_std_vector< std::vector<T> > : std::true_type {};

template <typename T> struct is_std_array : std::false_type {};
template <typename T, std::size_t size>
struct is_std_array< std::array<T, size> > : std::true_type {};

template <typename T> struct is_std_string : std::false_type {};
template <typename T>
struct is_std_string< std::basic_string<T> > : std::true_type {};
