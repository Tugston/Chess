#pragma once

//window wrapper class

class SDL_Window;

namespace GraphicsEngine {
	class Window {
	public:
		Window();
		Window(const float& windowSize);
		~Window();


		SDL_Window* GetWindow() { return this->window; };

	private:
		SDL_Window* window;
	};
}