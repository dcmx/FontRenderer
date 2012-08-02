/*
 * Object.h
 *
 *  Created on: 27.07.2012
 *      Author: Dominik S. Kaaser
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include <ft2build.h>
#include FT_FREETYPE_H
#include "line.h"
#include <list>
#include <string>
#include <iostream>
namespace dcm {

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
	void addLine(Line* line);
	double getWidth();
	double getHeight();
	Vector getPosition();
	void output(std::ostream& out);
	void input(std::istream& in);
	std::pair<Vector, Vector> getBoundingBox();
	void loadFace(char* filename);
	void setScaleToUnit(bool value);
	void scaleToUnit();
	void setLineHeight(double lineHeight);
	void setLetterSpacing(double letterSpacing);
	void setSampleRate(int sampleRate);
private:
	bool initialized;
	const int PRECISION;
	const double DEFAULTLINEHEIGHT;
	std::list<Line*> lines;
	FT_Library library;
	FT_Face face;
	unsigned char *buffer;
	double lineHeight;
	double letterSpacing;
	int sampleRate;
	bool doScaleToUnit;

};


} /* namespace dcm */
#endif /* OBJECT_H_ */
