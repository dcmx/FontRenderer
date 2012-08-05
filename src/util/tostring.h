/*
 * tostring.h
 *
 *  Created on: 04.08.2012
 *      Author: Dominik S. Kaaser
 */

#ifndef TOSTRING_H_
#define TOSTRING_H_
#include <string>
#include <sstream>
namespace dcm {
namespace util {
template<class T>
inline std::string toString(const T& t) {
	std::ostringstream ss;
	ss << t;
	return ss.str();
}
}
}
#endif /* TOSTRING_H_ */


