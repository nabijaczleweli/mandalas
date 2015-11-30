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


generator::generator() : rand(random_device{}()), past{} {}

void generator::generate_next(const sf::Vector2u & maxsize) {
	uniform_int_distribution<unsigned int> distx(0, maxsize.x);
	uniform_int_distribution<unsigned int> disty(0, maxsize.y);

	Vector2f pos(distx(rand), disty(rand));

	auto distances = make_array(distance(past[0].position, past[1].position), distance(past[1].position, past[2].position), distance(past[2].position, pos));
	sort(begin(distances), end(distances));
	Color col(distances[0] * 0xFF, distances[1] * 0xFF, distances[2] * 0xFF);

	Vertex newpoint(pos, col);

	iter_swap(past.begin(), past.begin() + 1);
	iter_swap(past.begin() + 1, past.begin() + 2);
	iter_swap(past.begin() + 2, &newpoint);
}


void generator::draw_latest(RenderTarget & on) {
	on.draw(past.end() - 1, 1, PrimitiveType::Points);
}
