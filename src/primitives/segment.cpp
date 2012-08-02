/*
 * segment.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: dkaaser
 */

#include "segment.h"

namespace dcm {

Segment::Segment(const Vector start, const Vector end) :
		start(start), end(end) {

}

Segment::~Segment() {

}

const int Segment::size() const {
	return 1;
}

const Vector Segment::getStartPoint() const {
	return start;
}
void Segment::output(std::ostream& out) const {
	out << end << std::endl;
}
void Segment::updateBoundingBox(Vector& leftBottom, Vector& rightTop) const {
	leftBottom.x = min3(leftBottom.x, start.x, end.x);
	leftBottom.y = min3(leftBottom.y, start.y, end.y);
	rightTop.x = max3(rightTop.x, start.x, end.x);
	rightTop.y = max3(rightTop.y, start.y, end.y);
}
} /* namespace dcm */
