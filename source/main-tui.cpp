// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include <tinyfiledialogs.h>
#include <tclap/CmdLine.h>
#include <iostream>
#include "util/suffixed_number_constraint.hpp"
#include "util/suffixed_number_parser.hpp"
#include "util/array.hpp"
#include "generator.hpp"


#ifdef _WIN32
#define DEVNULL "NUL"
#define DEVTTY "CON"
#else
#define DEVNULL "/dev/null"
#define DEVTTY "/dev/tty"
#endif


using namespace std;
using namespace TCLAP;


struct settings_t {
	string invocation_command;
	string output_file;
	unsigned long long int points_to_generate;
};


settings_t load_settings(int argc, const char * const * argv);


int main(int argc, const char ** argv) {
	const auto && settings = load_settings(argc, argv);

	freopen(DEVNULL, "w", stderr);  // Get rid of: "INFO: Could not find files for the given pattern(s)." (no, it's not related to file patterns)
	const auto saveto =
	    tinyfd_saveFileDialog("Save the generated mandala to...", "mandala.png", 4, make_array("*.bmp", "*.png", "*.tga", "*.jpg").data(), "image files");
	freopen(DEVTTY, "w", stderr);
}


settings_t load_settings(int argc, const char * const * argv) {
	settings_t ret;
	ret.invocation_command = argv[0];

	try {
		suffixed_number_constraint<unsigned long long int> points_to_generate_constraint({"h", "k", "M", "G"});

		CmdLine command_line("mandalas-tui", ' ', MANDALAS_VERSION);
		ValueArg<string> output_file("o", "output-file", "File the generated image will be output to; a prompt will be displayed otherwise", false, "",
		                             "path to image", command_line);
		ValueArg<string> points_to_generate("p", "points", "Amount of points to generate; can be suffixed with the standard SI suffixes", true, "0",
		                                    &points_to_generate_constraint, command_line);

		command_line.parse(argc, argv);

		ret.output_file        = output_file.getValue();
		ret.points_to_generate = parse_suffixed_number<unsigned long long int>(points_to_generate.getValue());
	} catch(const ArgException & e) {
		cerr << ret.invocation_command << ": error: parsing arguments failed (" << e.error() << ") for argument " << e.argId()
		     << "\ntrying to continue anyway.\n\n";
	}

	return ret;
}
