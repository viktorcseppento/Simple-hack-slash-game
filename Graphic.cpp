#include "Graphic.h"



Graphic::Graphic(int W, int H, std::string T) : screenWidth(W), screenHeight(H), title(T), window(NULL), windowSurface(NULL), SDLRunning(false)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw SDL_error("Couldn't initialize SDL");
	}
	else {
		SDLRunning = true;
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			throw SDL_error("Couldn't initialize window");
		}
		else {
			windowSurface = SDL_GetWindowSurface(window);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
				throw SDL_error("Couldn't initialize renderer");
			else {
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);
				if (TTF_Init() == -1)
					throw TTF_error("Couldn't initialize SDL_ttf");
			}
		}
	}
}


Graphic::~Graphic()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;
	windowSurface = NULL;
	TTF_Quit();
	SDL_Quit();
}

const int Graphic::getScreenWidth() const
{
	return screenWidth;
}

const int Graphic::getScreenHeight() const
{
	return screenHeight;
}

const std::string Graphic::getTitle() const
{
	return title;
}

SDL_Window * Graphic::getWindow()
{
	return window;
}

SDL_Surface * Graphic::getWindowSurface()
{
	return windowSurface;
}

SDL_Renderer * Graphic::getRenderer()
{
	return renderer;
}


bool Graphic::isRunning()
{
	return SDLRunning;
}

void Graphic::renderTexture(Texture& texture, double x, double y, double angle) //texture referencia, x,y pozíció, fordítási szög
{
	SDL_Rect renderingRect = { (int)x, (int)y, texture.getWidth(), texture.getHeight() };
	SDL_RenderCopyEx(renderer, texture.getTexture(), NULL, &renderingRect, angle, NULL, SDL_FLIP_NONE);
}

void Graphic::renderText(SDL_Texture * textTexture, double x, double y, int width, int height)
{
	SDL_Rect renderingRect = { (int)x, (int)y, width, height };
	SDL_RenderCopy(renderer, textTexture, NULL, &renderingRect);
}
