/*
 * primitive.h
 *
 *  Created on: Jul 30, 2012
 *      Author: dkaaser
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_
#include <iostream>
#define min3(a,b,c) std::min(a, std::min(b, c));
#define max3(a,b,c) std::max(a, std::max(b, c));
#include "vector.h"
namespace dcm {
class Primitive {
public:
	Primitive() { }
	virtual ~Primitive() { }
	virtual const int size() const = 0;
	virtual const Vector getStartPoint() const = 0;
	virtual void output(std::ostream& out) const = 0;
	virtual void updateBoundingBox(Vector& leftBottom, Vector& rightTop) const = 0;
};
}
#endif /* PRIMITIVE_H_ */
