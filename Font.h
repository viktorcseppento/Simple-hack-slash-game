#pragma once

#include <SDL\SDL.h>
#include <iostream>
#include <string>
#include "Graphic.h"

class Graphic;

class Font //Ez az osztály azért kell, hogy pl. mindig kiírjuk a játékos jelenlegi életerejét
{
	int size; //Betűk mérete
	TTF_Font *font; //SDL TTF moduljának a font változója
	SDL_Texture *textTexture; //Mindig tartalmaz egy szöveget, amit majd kiír/kirajzol
	int width;
	int height;
	int posX;
	int posY; //Jelenlegi szövegnek méretei, koordinátái
	Graphic *g; //Grafikus objektumra mutató pointer
	SDL_Color color; //Szöveg színe
public:
	Font(Graphic *g = NULL, int size = 24); //.ttf fájl betöltése
	~Font();
	void loadText(std::string text); //Egy szöveget betölt textúraként
	void drawText(std::string text, int posXParam, int posYParam, SDL_Color newColor); //Szöveg kirajzolása
	void freeFont(); //Betűtípus felszabadítása
	void setFontSize(int sizeParam); //Betűméret beállítása
};

