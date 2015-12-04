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


#include <boost/core/demangle.hpp>
#include <tclap/CmdLine.h>
#include <initializer_list>
#include <iterator>
#include <typeinfo>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


template <class T>
class suffixed_number_constraint : public TCLAP::Constraint<std::string> {
private:
	std::vector<std::string> possibles;

public:
	virtual std::string description() const override {
		return '`' + boost::core::demangle(typeid(T).name()) + "`s suffixed with a string.";  // TODO: List keys here?
	}

	virtual std::string shortID() const override {
		return std::accumulate(possibles.begin(), possibles.end(), boost::core::demangle(typeid(T).name()) + " suffixed with any of:",
		                       [](const auto & cur, const auto & el) { return cur + " \"" + el + "\""; });
	}

	virtual bool check(const std::string & value) const override {
		std::istringstream strm(value);

		T val;
		strm >> val;
		std::string suff(std::istreambuf_iterator<char>(strm), {});

		return std::find(possibles.begin(), possibles.end(), suff) != possibles.end();
	}

	template <class Itr>
	suffixed_number_constraint(Itr beg, Itr end) : possibles(beg, end) {}
	suffixed_number_constraint(std::initializer_list<std::string> init) : possibles(init) {}

	virtual ~suffixed_number_constraint() = default;
};
