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


#include <iostream>
#include <utility>


// error: call to function 'operator>>' that is neither visible in the template definition nor found by argument-dependent lookup
std::istream & operator>>(std::istream & strm, std::pair<unsigned int, unsigned int> & into);


#include <tinyfiledialogs.h>
#include <tclap/CmdLine.h>
#include <memory>
#include <chrono>
#include "util/extensioned_path_constraint.hpp"
#include "util/suffixed_number_constraint.hpp"
#include "util/suffixed_number_parser.hpp"
#include "util/array.hpp"
#include "util/video.hpp"
#include "generator.hpp"


#ifdef _WIN32
#define DEVNULL "NUL"
#define DEVTTY "CON"
#else
#define DEVNULL "/dev/null"
#define DEVTTY "/dev/tty"
#endif


using namespace sf;
using namespace std;
using namespace std::chrono;
using namespace TCLAP;


struct settings_t {
	string invocation_command;
	unique_ptr<string> output_file;
	pair<unsigned int, unsigned int> dimensions;
	unsigned long long int points_to_generate;
};


settings_t load_settings(int argc, const char * const * argv);


int main(int argc, const char ** argv) {
	auto && settings = load_settings(argc, argv);

	while(!settings.output_file) {
		freopen(DEVNULL, "w", stderr);  // Get rid of: "INFO: Could not find files for the given pattern(s)." (no, it's not related to file patterns)
		const auto saveto =
		    tinyfd_saveFileDialog("Save the generated mandala to...", "mandala.png", 4, make_array("*.bmp", "*.png", "*.tga", "*.jpg").data(), "image files");
		freopen(DEVTTY, "w", stderr);

		if(!saveto) {
			cerr << "User cancelled filename prompt; aborting\n";
			return 1;
		}

		settings.output_file = make_unique<string>(saveto);
		if(!extensioned_path_constraint{}.check(*settings.output_file)) {
			cout << "Specified path doesn't contain an extension; please retry\n\n";
			settings.output_file = nullptr;
		}
	}
}


settings_t load_settings(int argc, const char * const * argv) {
	settings_t ret;
	ret.invocation_command = argv[0];

	try {
		suffixed_number_constraint<unsigned long long int> points_to_generate_constraint({"h", "k", "M", "G"});
		extensioned_path_constraint output_file_constraint;

		CmdLine command_line("mandalas-tui -- the headless brother of mandalas-gui!", ' ', MANDALAS_VERSION);
		ValueArg<pair<unsigned int, unsigned int>> dimensions("s", "size", "Output file size", false, max_square_video_size(), "NxM", command_line);
		ValueArg<string> output_file("o", "output-file", "File the generated image will be output to; a prompt will be displayed otherwise", false, "",
		                             &output_file_constraint, command_line);
		ValueArg<string> points_to_generate("p", "points", "Amount of points to generate; can be suffixed with the standard SI suffixes", true, "0",
		                                    &points_to_generate_constraint, command_line);

		command_line.parse(argc, argv);

		ret.dimensions = dimensions.getValue();
		if(!output_file.getValue().empty())
			ret.output_file = make_unique<string>(output_file.getValue());
		ret.points_to_generate = parse_suffixed_number<unsigned long long int>(points_to_generate.getValue());
	} catch(const ArgException & e) {
		cerr << ret.invocation_command << ": error: parsing arguments failed (" << e.error() << ") for argument " << e.argId()
		     << "\ntrying to continue anyway.\n\n";
	}

	return ret;
}

istream & operator>>(istream & strm, pair<unsigned int, unsigned int> & into) {
	unsigned int x;
	unsigned int y;
	char temp;

	strm >> x >> temp >> y;

	if(temp != 'x')
		strm.setstate(ios::failbit);

	return strm;
}
