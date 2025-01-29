#pragma once

struct SDL_Window;

namespace GraphicsEngine {
	class Window {
	public:
		Window();
		Window(const int& windowSize);
		~Window();
		SDL_Window* GetWindow() { return this->window; };
	private:
		SDL_Window* window;
	};
}