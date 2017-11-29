#pragma once
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

template <typename T>
inline std::vector<T> operator+(const std::vector<T> &A, const std::vector<T> &B)
{
	std::vector<T> AB;
	AB.reserve(A.size() + B.size());                // preallocate memory
	AB.insert(AB.end(), A.begin(), A.end());        // add A;
	AB.insert(AB.end(), B.begin(), B.end());        // add B;
	return AB;
}

template <typename T>
inline std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B)
{
	A.reserve(A.size() + B.size());                // preallocate memory without erase original data
	A.insert(A.end(), B.begin(), B.end());         // add B;
	return A;                                        // here A could be named AB
}

template<typename Container>
inline bool exist_in(const Container& c, const typename Container::value_type& v) {
	return (c.end() != std::find(c.begin(), c.end(), v));
}

template <class Container, class Function>
inline auto apply(const Container &cont, Function fun) {
	std::vector< typename
		std::result_of<Function(const typename Container::value_type&)>::type> ret;
	ret.reserve(cont.size());
	std::transform(cont.begin(), cont.end(), std::back_inserter(ret), fun);
	return ret;
}

inline string tohex(uint32_t x) {
	ostringstream ostr;
	ostr << hex << setfill('0') << setw(8) << x;
	return ostr.str();
}
