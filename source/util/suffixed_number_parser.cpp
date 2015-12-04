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


#include <stdexcept>
#include <sstream>
#include <string>


using namespace std;


unsigned long long int parse_suffix(const string & suff) {
	if(suff.empty())
		return 1;

	if(suff.size() == 1)
		switch(suff[0]) {
			// clang-format off
			case 'h': return 100;
			case 'k': return 1000;
			case 'M': return 1000000;
			case 'G': return 1000000000ull;
			case 'T': return 1000000000000ull;
				// clang-format on
		}

	if(suff == "da")
		return 10;

	throw invalid_argument('"' + suff + "\" isn't one of the recognized SI suffixes");
}
