//============================================================================
// Name        : fontrenderer.cpp
// Author      : Dominik S. Kaaser
// Version     : 1.0.0
// Copyright   : (C) Copyright 2012
// Description : Font-Renderer
//============================================================================

#define VERSION "1.0.0"
#define NAME "FontRenderer"
#include <iostream>
#include <istream>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <memory>

#include "renderer.h"

using namespace dcm;
using namespace std;

static string programName;

static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'v' },
		{ "line-height", required_argument, 0, 'l' },
		{ "letter-spacing", required_argument, 0, 'w' },
		{ "sample-rate", required_argument, 0, 's' },
		{ "no-scale", required_argument, 0, 'u' },
		{ NULL, 0, 0, 0 }
};

static string options_descriptions[] = {
		"show this help",
		"print version info and exit",
		"set line height",
		"set letter spacing",
		"set sample rate for Bezier curves",
		"do not scale to unit square",
		""
};

static string options_params[] = {
		"",
		"",
		"height",
		"width",
		"samples",
		"",
		""
};

static const int usage(const int error = 0) {

	ostream& out = error ? cerr : cout;

	out << NAME << ": Converts a text to polygonal data in line format using a" << endl
			<< "specified font." << endl
			<< endl
			<< "Usage: " << programName << " [options] fontfile [input [output]]" << endl
			<< endl
			<< "Options:" << endl;

	int i = 0;
	while (long_options[i].val) {
		out << "    -" << (char) long_options[i].val << " " << options_params[i] << endl << "    " << "--" << left
				<< setw(16) << long_options[i].name << options_descriptions[i] << endl << endl;
		i++;
	}
	return error;
}

int main(int argc, char** argv) {

	try {
		programName = string(argv[0]);

		Renderer* object = new Renderer();

		char* font;
		istream* input = &cin;
		ostream* output = &cout;
		auto_ptr<ifstream> fileInputStream;
		auto_ptr<ofstream> fileOutputStream;

		int opt;
		int index = 0;

		while ((opt = getopt_long(argc, argv, "hvl:w:s:u", long_options, &index)) != -1) {

			switch (opt) {
			case 'h':
				delete object;
				return usage();
			case 'u':
				object->setScaleToUnit(false);
				break;
			case 'l':
				object->setLineHeight(atof(optarg));
				break;
			case 'w':
				object->setLetterSpacing(atof(optarg));
				break;
			case 's':
				object->setSampleRate(atoi(optarg));
				break;
			case 'v':
				cout << NAME << " version " << VERSION << endl << "Copyright (C) 2012 Dominik S. Kaaser" << endl
						<< "http://font.kaaser.at - dominik@kaaser.at" << endl;
				return 0;
			case '?':
				delete object;
				return usage(1);
			default:
				abort();
				break;
			}
		}

		switch (argc - optind) {
		case 3:
			if (strcmp(argv[optind + 2], "-") != 0) {
				fileOutputStream = auto_ptr<ofstream>(new ofstream(argv[optind + 2], ios::out | ios::trunc));
				if (!fileOutputStream->is_open())
					throw(string("Could not open output file: ") + string(argv[optind + 2]));
				output = fileOutputStream.get();
			}
			/* no break */
		case 2:
			if (strcmp(argv[optind + 1], "-") != 0) {
				fileInputStream = auto_ptr<ifstream>(new ifstream(argv[optind + 1], ios::in));
				if (!fileInputStream->is_open())
					throw(string("Could not open input file: ") + string(argv[optind + 1]));
				input = fileInputStream.get();
			}
			/* no break */
		case 1:
			font = argv[optind];
			break;
		default:
			return usage(1);
		}

		object->loadFace(font);

		object->input(*input);

		object->output(*output);

		delete object;

	} catch (string &exception) {
		cerr << exception << endl;
		return 2;
	}

	return 0;
}
