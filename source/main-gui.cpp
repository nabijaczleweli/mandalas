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
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "util/array.hpp"
#include "util/video.hpp"
#include "generator.hpp"


using namespace sf;
using namespace std;

int main() {
	RenderWindow window(max_square_video_mode(), "mandalas", Style::None);
	window.clear();
	Font inconsolata;
	inconsolata.loadFromFile("assets/Inconsolata.otf");
	window.draw(Text("Press 1-0 to generate 10^(n+1) [where n=number key's distance from the 1 key]\n"
	                 "Press Enter to save the mandala\n"
	                 "\n"
	                 "Press any key to continue\n",
	                 inconsolata, 15));
	window.display();

	Event event;
	for(;;) {
		window.waitEvent(event);
		if(event.type == Event::KeyPressed)
			break;
		else if(event.type == Event::Closed) {
			window.close();
			break;
		}
	}
	window.clear();
	window.display();

	generator gen(window.getSize() / 2u);
	while(window.isOpen() && window.waitEvent(event)) {
		if(event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Key::Escape))
			window.close();
		else if(event.type == Event::KeyPressed) {
			auto to_gen = 0ull;

			switch(event.key.code) {
				case Keyboard::Key::Num1:
				case Keyboard::Key::Numpad1:
					to_gen = 10ull;
					break;
				case Keyboard::Key::Num2:
				case Keyboard::Key::Numpad2:
					to_gen = 100ull;
					break;
				case Keyboard::Key::Num3:
				case Keyboard::Key::Numpad3:
					to_gen = 1000ull;
					break;
				case Keyboard::Key::Num4:
				case Keyboard::Key::Numpad4:
					to_gen = 10000ull;
					break;
				case Keyboard::Key::Num5:
				case Keyboard::Key::Numpad5:
					to_gen = 100000ull;
					break;
				case Keyboard::Key::Num6:
				case Keyboard::Key::Numpad6:
					to_gen = 1000000ull;
					break;
				case Keyboard::Key::Num7:
				case Keyboard::Key::Numpad7:
					to_gen = 10000000ull;
					break;
				case Keyboard::Key::Num8:
				case Keyboard::Key::Numpad8:
					to_gen = 100000000ull;
					break;
				case Keyboard::Key::Num9:
				case Keyboard::Key::Numpad9:
					to_gen = 1000000000ull;
					break;
				case Keyboard::Key::Num0:
				case Keyboard::Key::Numpad0:
					to_gen = 10000000000ull;
					break;
				case Keyboard::Key::S:
					if(!(Keyboard::isKeyPressed(Keyboard::Key::LControl) || Keyboard::isKeyPressed(Keyboard::Key::RControl)))
						break;
				case Keyboard::Key::Return: {
					const auto saveto =
					    tinyfd_saveFileDialog("Save the generated mandala to...", "mandala.png", 4, make_array("*.bmp", "*.png", "*.tga", "*.jpg").data(), "image files");
					if(saveto)
						window.capture().saveToFile(saveto);
				} break;
				default:
					continue;
			}

			if(to_gen) {
				gen.generate_n(window.getSize(), to_gen);
				gen.draw_n(window, to_gen);
			}
		} else
			continue;

		window.display();
	}
}
