/*
 * Line.h
 *
 *  Created on: 27.07.2012
 *      Author: Dominik S. Kaaser
 */

#ifndef LINE_H_
#define LINE_H_
#include <ft2build.h>
#include FT_FREETYPE_H
#include <list>
#include <string>
#include "primitive.h"
#include "primitives/segment.h"

#define SHIFT 20
#define UNSCALE (1.0 / (double)(1 << SHIFT))

#include "vector.h"
namespace dcm {

class Line {
public:
	Line(const std::string line);
	virtual ~Line();
	void render(FT_Face &face, const double posY, const double spacing);
	void output(std::ostream& out) const;
	const Vector getFirstVertex() const;
	void updateBoundingBox(Vector& leftBottom, Vector& rightTop) const;
private:
	const std::string line;

	std::list<std::list<const Primitive*> > primitives;

	Vector position;
	Vector offset;

	void moveTo(const Vector to);
	void lineTo(const Vector to);
	void conicTo(const Vector control, const Vector to);
	void cubicTo(const Vector control1, const Vector control2, const Vector to);

	static int wrapMoveTo(const FT_Vector* to, void* user);
	static int wrapLineTo(const FT_Vector* to, void* user);
	static int wrapConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
	static int wrapCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);

};

} /* namespace dcm */
#endif /* LINE_H_ */
