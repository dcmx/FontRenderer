/*
 * vector.cpp
 *
 *  Created on: Jul 31, 2012
 *      Author: dkaaser
 */
#include "vector.h"
namespace dcm {
Vector Vector::shift(0.0, 0.0);
double Vector::scale = 1.0;
std::ostream &operator<<(std::ostream &stream, Vector ob) {
	stream << std::setw(25) << std::right << (ob.x + Vector::shift.x) * Vector::scale << " " << std::setw(25) << std::right << (ob.y + Vector::shift.y)*Vector::scale;
	return stream;
}

}

