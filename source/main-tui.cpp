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
#include <iostream>
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


int main(int, const char ** argv) {
	freopen(DEVNULL, "w", stderr);  // Get rid of: "INFO: Could not find files for the given pattern(s)." (no, it's not related to file patterns)
	const auto saveto =
	    tinyfd_saveFileDialog("Save the generated mandala to...", "mandala.png", 4, make_array("*.bmp", "*.png", "*.tga", "*.jpg").data(), "image files");
	freopen(DEVTTY, "w", stderr);
	cout << (saveto ?: "NULL") << '\n';
}
