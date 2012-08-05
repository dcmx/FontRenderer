/*
 * Line.cpp
 *
 *  Created on: 27.07.2012
 *      Author: Dominik S. Kaaser
 */

#include <assert.h>
#include <sstream>

#include "line.h"
#include "primitive.h"
#include "primitives/segment.h"
#include "primitives/bezier.h"

#include "util/tostring.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#ifdef USE_UTF8
#include <unicode/unistr.h>
#include <unicode/chariter.h>
#include <unicode/schriter.h>
#endif

using namespace dcm::util;

namespace dcm {

Line::Line(const std::string line) :
		line(line), offset(0.0, 0.0) {
}

Line::~Line() {

}

void Line::output(std::ostream& out) const {

	for (std::list<std::list<const Primitive*> >::const_iterator it1 = primitives.begin(); it1 != primitives.end(); ++it1) {

		int size = 1;
		for (std::list<const Primitive*>::const_iterator it2 = (it1)->begin(); it2 != (it1)->end(); ++it2)
			size += (*it2)->size();
		out << size << std::endl;
		out << it1->front()->getStartPoint() << std::endl;
		for (std::list<const Primitive*>::const_iterator it2 = (it1)->begin(); it2 != (it1)->end(); ++it2)
			(*it2)->output(out);

	}
}

int Line::wrapMoveTo(const FT_Vector* to, void* user) {
	Line* line = (Line*) user;
	line->moveTo(Vector(((double) to->x) * UNSCALE, ((double) to->y) * UNSCALE));
	return 0;
}
int Line::wrapLineTo(const FT_Vector* to, void* user) {
	Line* line = (Line*) user;
	line->lineTo(Vector(((double) to->x) * UNSCALE, ((double) to->y) * UNSCALE));
	return 0;
}
int Line::wrapConicTo(const FT_Vector* control, const FT_Vector* to, void* user) {
	Line* line = (Line*) user;
	line->conicTo(Vector(((double) control->x) * UNSCALE, ((double) control->y) * UNSCALE),
			Vector(((double) to->x) * UNSCALE, ((double) to->y) * UNSCALE));
	return 0;
}
int Line::wrapCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user) {
	Line* line = (Line*) user;
	line->cubicTo(Vector(((double) control1->x) * UNSCALE, ((double) control1->y) * UNSCALE),
			Vector(((double) control2->x) * UNSCALE, ((double) control2->y) * UNSCALE),
			Vector(((double) to->x) * UNSCALE, ((double) to->y) * UNSCALE));
	return 0;
}

void Line::moveTo(const Vector to) {
	position = to;
	primitives.push_back(std::list<const Primitive*>());
}
void Line::lineTo(const Vector to) {
	primitives.back().push_back(new Segment(position + offset, to + offset));
	position = to;
}
void Line::conicTo(const Vector control, const Vector to) {
	std::list<Vector> vertices;
	vertices.push_back(position + offset);
	vertices.push_back(control + offset);
	vertices.push_back(to + offset);
	primitives.back().push_back(new Bezier(vertices, 2));
	position = to;

}
void Line::cubicTo(const Vector control1, const Vector control2, const Vector to) {
	std::list<Vector> vertices;
	vertices.push_back(position + offset);
	vertices.push_back(control1 + offset);
	vertices.push_back(control2 + offset);
	vertices.push_back(to + offset);
	primitives.back().push_back(new Bezier(vertices, 3));
	position = to;
}

void Line::render(FT_Face &face, const double posY, const double spacing) {
	offset.y = posY;

#ifdef USE_UTF8
	UnicodeString unicodeLine = UnicodeString::fromUTF8(StringPiece(line));
	StringCharacterIterator it(unicodeLine);

	for (it.setToStart(); it.hasNext();) {
		UChar32 ch = it.next32PostInc();

#else
		for(std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
			char ch = *it;
#endif

		FT_UInt gindex = FT_Get_Char_Index(face, ch);
		if (FT_Load_Glyph(face, gindex, FT_LOAD_NO_BITMAP))
			throw std::string("Could not load glyph for character nr. ") + toString(ch);
		if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
			throw std::string("The selected glyph within the currently loaded face does not have an outline");
		FT_Glyph glyph;
		if (FT_Get_Glyph(face->glyph, &glyph))
			throw std::string("Unable to load Glyph");

		FT_Outline& outline = face->glyph->outline;

		FT_Outline_Funcs funcs;
		funcs.move_to = Line::wrapMoveTo;
		funcs.line_to = Line::wrapLineTo;
		funcs.conic_to = Line::wrapConicTo;
		funcs.cubic_to = Line::wrapCubicTo;
		funcs.delta = 0;
		funcs.shift = 0;

		if (FT_Outline_Decompose(&outline, &funcs, this))
			throw std::string("Unable to decompose the outline.");

		offset.x += (((double) face->glyph->advance.x) * UNSCALE) * spacing;

		FT_Done_Glyph(glyph);
	}
}
const Vector Line::getFirstVertex() const {
	if (primitives.size() == 0 || primitives.front().size() == 0)
		return Vector();
	return primitives.front().front()->getStartPoint();
}

void Line::updateBoundingBox(Vector& leftBottom, Vector& rightTop) const {
	for (std::list<std::list<const Primitive*> >::const_iterator it1 = primitives.begin(); it1 != primitives.end(); ++it1)
		for (std::list<const Primitive*>::const_iterator it2 = (it1)->begin(); it2 != (it1)->end(); ++it2)
			(*it2)->updateBoundingBox(leftBottom, rightTop);
}
} /* namespace dcm */
