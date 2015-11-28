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


#include "generator.hpp"
#include "util/math.hpp"
#include "util/array.hpp"
#include <algorithm>


using namespace sf;
using namespace std;


template <class Gen>
static void gen(Gen && gen, mt19937 & rand, vector<Vertex> & past) {
	auto && to   = gen(rand);
	auto && prev = *(past.end() - 1);
	Vector2f pos(prev.position.x - ((prev.position.x - to.x) / 2), prev.position.y - ((prev.position.y - to.y) / 2));

	auto distances = make_array(distance((past.end() - 4)->position, (past.end() - 3)->position),  //
	                            distance((past.end() - 3)->position, (past.end() - 2)->position),  //
	                            distance((past.end() - 2)->position, prev.position),               //
	                            distance(prev.position, pos));
	sort(begin(distances), end(distances));
	Color col(abs(0xFF - round((distances[0] / distances[3]) * 0xFF)),  //
	          abs(0xFF - round((distances[1] / distances[3]) * 0xFF)),  //
	          abs(0xFF - round((distances[2] / distances[3]) * 0xFF)));

	past.emplace_back(pos, col);
}

generator::generator() : rand(random_device{}()), past(4) {}

void generator::generate_next(const sf::Vector2u & maxsize) {
	generate_n(maxsize, 1);
}

void generator::generate_n(const sf::Vector2u & maxsize, long unsigned long int n) {
	const auto dist = [&](auto & rand) -> Vector2u {
		uniform_int_distribution<unsigned int> d(0, 3);
		switch(d(rand)) {
			case 0:
				return {0, 0};
			case 1:
				return {maxsize.x - 1, 0};
			case 2:
				return {0, maxsize.y - 1};
			case 3:
				return {maxsize.x - 1, maxsize.y - 1};
			default:  // impossible
				return {};
		}
	};

	past.reserve(past.capacity() + n);

	for(auto i = 0ull; i < n; ++i)
		gen(dist, rand, past);
}

void generator::draw_n(sf::RenderTarget & on, unsigned long long int n) {
	on.draw(&*(past.end() - n - 1), n, PrimitiveType::Points);
}

void generator::draw_latest(RenderTarget & on) {
	draw_n(on, 1);
}
