#ifndef HEADER_TENUIS_COMMON_HPP_
#define HEADER_TENUIS_COMMON_HPP_

namespace tenuis {

template <class Lhs, class Rhs, bool>
struct left_or_right;

template <class Lhs, class Rhs>
struct left_or_right<Lhs, Rhs, true> : Lhs {};

template <class Lhs, class Rhs>
struct left_or_right<Lhs, Rhs, false> : Rhs {};

template <class Lhs, class Rhs>
struct max_value : left_or_right<Lhs, Rhs, (Lhs::value > Rhs::value)> {};

template <class A, class B>
struct max_size : max_value<
	std::integral_constant<std::size_t, sizeof(A)>,
	std::integral_constant<std::size_t, sizeof(B)>> {};

template <class A, class B>
struct max_align : max_value<
	std::integral_constant<std::size_t, alignof(A)>,
	std::integral_constant<std::size_t, alignof(B)>> {};

}

#endif