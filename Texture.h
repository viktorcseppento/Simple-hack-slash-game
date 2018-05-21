#pragma once

#include <SDL\SDL.h>
#include <iostream>
#include <string>
#include "Graphic.h"
#include "Map.h"

class Graphic;

class Texture
{
	int width; //Méretek
	int height;
	SDL_Texture *texture; //SDL elemi textúrája
	Graphic *g; //A grafikus objektumra pointer
	int mapping; //0 = no mapping, 1 = black mapping, 2 = white mapping; ez arra való, hogy a négyzet alakú textúrának csak a fontos részei látszódjanak a képernyőn
public:
	Texture(Graphic* g = NULL, std::string path = NULL, int width = 24, int height = 24, int mapping= 0);
	~Texture();
	void loadFromFile(std::string path); //Fájlból történő olvasás (.bmp fájl)
	void freeTexture(); //Területek felszabadítása
	int getWidth() const;
	int getHeight() const;
	void setWidth(int w);
	void setHeight(int h);
	SDL_Texture* getTexture() const;
};