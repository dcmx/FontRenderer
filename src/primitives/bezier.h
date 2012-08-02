/*
 * bezier.h
 *
 *  Created on: Jul 30, 2012
 *      Author: dkaaser
 */

#ifndef BEZIER_H_
#define BEZIER_H_

#include "../primitive.h"
#include "../vector.h"
#include <list>
namespace dcm {

class Bezier: public Primitive {
public:
	Bezier(std::list<Vector> vertices, int degree);
	virtual ~Bezier();
	virtual const int size() const;
	virtual const Vector getStartPoint() const;
	virtual void output(std::ostream& out) const;
	virtual void updateBoundingBox(Vector& leftBottom, Vector& rightTop) const;
	static void setSampleRate(int sampleRate);
private:
	static unsigned int sampleRate;
	std::list<Vector> vertices;
	int degree;
	double length;
	Vector evaluate(const double t) const;
};

} /* namespace dcm */
#endif /* BEZIER_H_ */
