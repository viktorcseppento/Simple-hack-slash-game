#pragma once

#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Texture.h"
#include "SDL_error.h"
#include "TTF_error.h"

class Texture;

class Graphic
{
	const int screenWidth; //Ablak paraméterei
	const int screenHeight;
	const std::string title; //Ablak címe
	SDL_Window *window; //Maga az elemi SDL ablak
	SDL_Surface *windowSurface; //Segédváltozó lesz a textúrák alkotásánál
	SDL_Renderer *renderer; //Ezzel égetjük a textúrákat az ablakra
	bool SDLRunning; //Igaz/hamis változó, hogy fut-e a grafikus felület
public:
	Graphic(int W = 800, int H = 600, std::string T = "Hack And Slash");
	~Graphic();
	const int getScreenWidth() const;
	const int getScreenHeight() const;
	const std::string getTitle() const;
	SDL_Window* getWindow();
	SDL_Surface* getWindowSurface();
	SDL_Renderer* getRenderer();
	bool isRunning();
	void renderTexture(Texture& texture, double x, double y, double angle = 0); //Egy textúrát ráéget az ablakra a megfelelőt helyre, szögben
	void renderText(SDL_Texture* textTexture, double x, double y, int width, int height); //Egy szövegtextúrák éget az ablakra
};

