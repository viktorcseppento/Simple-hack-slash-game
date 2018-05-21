#pragma once

#include <SDL\SDL.h>
#include <iostream>
#include <string>

class SDL_error :
	public std::exception
{
	std::string explanatory; //Elmagyarázza a hiba okát
public:
	SDL_error(std::string explanatory = "SDL_ERROR");
	virtual void what(); //Visszaadja a stringet + az SDL_Geterror() függvényt
	~SDL_error();
};

