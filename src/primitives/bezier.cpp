/*
 * bezier.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: dkaaser
 */

#include "bezier.h"

namespace dcm {

unsigned int Bezier::sampleRate;

void Bezier::setSampleRate(int sampleRate) {
	Bezier::sampleRate = sampleRate;
}

Bezier::Bezier(std::list<Vector> vertices, int degree) :
		vertices(vertices), degree(degree), length(0.0) {

	for (std::list<Vector>::iterator it2 = vertices.begin(), it1 = it2++; it2 != vertices.end(); ++it1, ++it2)
		length += ((*it2) - (*it1)).length();

}

Bezier::~Bezier() {
	vertices.clear();
}
const Vector Bezier::getStartPoint() const {
	return vertices.front();
}
const int Bezier::size() const {
	int samples = (int) std::max(4.0, length * (double) sampleRate);
	return samples - 1;
}
void Bezier::output(std::ostream& out) const {
	int samples = (int) std::max(4.0, length * (double) sampleRate);
	double sampleLength = 1.0 / samples;
	for (int i = 1; i < samples - 1; i++)
		out << evaluate(sampleLength * (double) i) << std::endl;

	out << vertices.back() << std::endl;

}

Vector Bezier::evaluate(const double t) const {
	std::list<Vector>::const_iterator it = vertices.begin();
	Vector A = *it;
	it++;
	Vector B = *it;
	it++;
	Vector C = *it;
	it++;

	if (degree == 2) {
		return A * (1.0 - t) * (1.0 - t) + B * 2.0 * (1.0 - t) * t + C * t * t;
	} else if (degree == 3) {
		Vector D = *it;
		return A * (1.0 - t) * (1.0 - t) * (1.0 - t) + B * 3.0 * (1.0 - t) * (1.0 - t) * t + C * 3.0 * (1.0 - t) * t * t
				+ D * t * t * t;
	}
	return Vector();
}
void Bezier::updateBoundingBox(Vector& leftBottom, Vector& rightTop) const {
	for (std::list<Vector>::const_iterator it = ++(vertices.begin()); it != vertices.end(); ++it) {
		if (it->x < leftBottom.x)
			leftBottom.x = it->x;
		if (it->y < leftBottom.y)
			leftBottom.y = it->y;
		if (it->x > rightTop.x)
			rightTop.x = it->x;
		if (it->y > rightTop.y)
			rightTop.y = it->y;
	}
}
} /* namespace dcm */
