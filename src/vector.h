#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iomanip>
#include <iostream>
namespace dcm {

class Vector {
public:

	Vector() :
			x(0.0), y(0.0) {
	}
	Vector(double x, double y) :
			x(x), y(y) {
	}
	Vector(const Vector& v) :
			x(v.x), y(v.y) {
	}

	inline bool operator==(const Vector& v) const {
		return x == v.x && y == v.y;
	}

	inline bool operator!=(const Vector& v) const {
		return x != v.x || y != v.y;
	}

	inline void operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
	}

	inline void operator*=(const double& s) {
		x *= s;
		y *= s;
	}

	inline Vector operator+(const Vector& v) const {
		return Vector(x + v.x, y + v.y);
	}

	inline Vector operator-(const Vector& v) const {
		return Vector(x - v.x, y - v.y);
	}

	inline Vector operator-() const {
		return Vector(-x, -y);
	}

	inline Vector operator*(const double& s) const {
		return Vector(x * s, y * s);
	}

	inline Vector operator/(const double& s) const {
		return Vector(x / s, y / s);
	}

	void normalize() {
		double l = length();
		x /= l;
		y /= l;
	}

	inline double length() const {
		return sqrt(x * x + y * y);
	}

	inline double length2() const {
		return (x * x + y * y);
	}

	inline double distance(const Vector&v) const {
		return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y));
	}

	inline double distance2(const Vector&v) const {
		return (v.x - x) * (v.x - x) + (v.y - y) * (v.y - y);
	}

	inline double operator*(const Vector& v) const {
		return (x * v.x) + (y * v.y);
	}

	double x;
	double y;

	friend std::ostream &operator<<(std::ostream &stream, Vector ob);
	static Vector shift;
	static double scale;
};

std::ostream &operator<<(std::ostream &stream, Vector ob);

inline Vector operator*(const double& s, const Vector&v) {
	return Vector(v.x * s, v.y * s);
}

}
#endif
