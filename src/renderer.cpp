/*
 * Object.cpp
 *
 *  Created on: 27.07.2012
 *      Author: Dominik S. Kaaser
 */

#include "renderer.h"
#include "primitives/bezier.h"
#include <iostream>
#include <istream>
#include <iomanip>
#include <fstream>

namespace dcm {

Renderer::Renderer() :
		PRECISION(20), DEFAULTLINEHEIGHT(-1.8), lineHeight(1.0), letterSpacing(1.0), sampleRate(100), doScaleToUnit(
				true) {

	if (FT_Init_FreeType(&library))
		throw std::string("Unable to initialize FreeType library");

}

void Renderer::setScaleToUnit(bool doScaleToUnit) {
	this->doScaleToUnit = doScaleToUnit;
}
void Renderer::setLineHeight(double lineHeight) {
	this->lineHeight = lineHeight;
}
void Renderer::setSampleRate(int sampleRate) {
	this->sampleRate = sampleRate;
}
void Renderer::setLetterSpacing(double letterSpacing) {
	this->letterSpacing = letterSpacing;
}

Renderer::~Renderer() {

	if (initialized) {
		if (FT_Done_Face(face))
			throw std::string("Unable to deallocate font face");
		delete[] buffer;
	}
	if (FT_Done_FreeType(library))
		throw std::string("Unable to deallocate FreeType library");

	for (std::list<Line*>::iterator it = lines.begin(); it != lines.end(); ++it)
		delete *it;
	lines.clear();

}

void Renderer::addLine(Line* line) {

	double posY = (double) lines.size() * DEFAULTLINEHEIGHT * lineHeight;
	lines.push_back(line);
	line->render(face, posY, letterSpacing);

}

void Renderer::loadFace(char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file)
		throw(std::string("Could not open font file: ") + std::string(filename));

	// Read the entire file to a memory buffer.
	file.seekg(0, std::ios::end);
	std::fstream::pos_type fileSize = file.tellg();
	file.seekg(0);
	buffer = new unsigned char[fileSize];
	file.read((char *) buffer, fileSize);

	// Create a face from a memory buffer.  Be sure not to delete the memory
	// buffer until you are done using that font as FreeType will reference
	// it directly.

	if (FT_New_Memory_Face(library, buffer, fileSize, 0, &face))
		throw std::string("Could not read font file (Format Error?)");

	initialized = true;

	if (FT_Set_Char_Size(face, 1 << SHIFT, 0, 100, 0))
		throw std::string("Could not set the char size.");

}

void Renderer::output(std::ostream& out) {
	out << std::setprecision(PRECISION) << std::fixed;
	if (doScaleToUnit)
		scaleToUnit();

	Bezier::setSampleRate(sampleRate);

	for (std::list<Line*>::iterator it = lines.begin(); it != lines.end(); ++it)
		(*it)->output(out);
}

void Renderer::input(std::istream& in) {
	while (!in.eof()) {
		char* tmp = new char[1024];
		in.getline(tmp, 1024);
		addLine(new Line(std::string(tmp)));
		delete[] tmp;
	}
}

std::pair<Vector, Vector> Renderer::getBoundingBox() {

	Vector leftBottom = lines.front()->getFirstVertex();
	Vector rightTop = lines.front()->getFirstVertex();
	for (std::list<Line*>::iterator it = lines.begin(); it != lines.end(); ++it)
		(*it)->updateBoundingBox(leftBottom, rightTop);
	return std::pair<Vector, Vector>(leftBottom, rightTop);
}
void Renderer::scaleToUnit() {
	std::pair<Vector, Vector> boundingBox = getBoundingBox();

	const double width = fabs(boundingBox.second.x - boundingBox.first.x);
	const double height = fabs(boundingBox.second.y - boundingBox.first.y);

	Vector::shift = -boundingBox.first;

	if (width > height) {
		Vector::scale = 1.0 / width;
		Vector::shift.y += (width - height) * 0.5;
	} else {
		Vector::scale = 1.0 / height;
		Vector::shift.x += (height - width) * 0.5;
	}
}
} /* namespace dcm */
