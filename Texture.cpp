#include "Texture.h"



Texture::Texture(Graphic *g, std::string path, int width, int height, int mapping) : width(width), height(height), texture(NULL), g(g), mapping(mapping)
{
	loadFromFile(path);
}


Texture::~Texture()
{
	freeTexture();
	g = NULL;
}

void Texture::loadFromFile(std::string path)
{
	freeTexture();
	SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
	SDL_Surface *optimized = NULL;
	if (loadedSurface == NULL)
		throw SDL_error("Unable to load image");
	else {
		optimized = SDL_ConvertSurface(loadedSurface, g->getWindowSurface()->format, 0);
		if (optimized == NULL)
			throw SDL_error("Unable to optimize image");
		else {
			SDL_FreeSurface(loadedSurface);
			if (mapping == 1)
				SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0x00, 0x00, 0x00));
			if (mapping == 2)
				SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0xFF, 0xFF, 0xFF));
			texture = SDL_CreateTextureFromSurface(g->getRenderer(), optimized);
			if (texture == NULL)
				throw SDL_error("Unable create texture");
			else
				SDL_FreeSurface(optimized);
		}
	}
}

void Texture::freeTexture()
{
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}

void Texture::setWidth(int w)
{
	width = w;
}

void Texture::setHeight(int h)
{
	height = h;
}

SDL_Texture * Texture::getTexture() const
{
	return texture;
}
