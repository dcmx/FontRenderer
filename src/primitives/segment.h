/*
 * segment.h
 *
 *  Created on: Jul 30, 2012
 *      Author: dkaaser
 */

#ifndef SEGMENT_H_
#define SEGMENT_H_
#include "../primitive.h"

namespace dcm {

class Segment: public Primitive {
public:
	Segment(const Vector start, const Vector end);
	virtual ~Segment();
	virtual const int size() const;
	virtual const Vector getStartPoint() const;
	virtual void output(std::ostream& out) const;
	virtual void updateBoundingBox(Vector& leftBottom, Vector& rightTop) const;
private:
	const Vector start;
	const Vector end;
};

} /* namespace dcm */
#endif /* SEGMENT_H_ */
