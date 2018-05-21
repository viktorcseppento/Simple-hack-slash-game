#pragma once

#include <SDL\SDL.h>
#include <iostream>
#include <string>

class SDL_error :
	public std::exception
{
	std::string explanatory; //Elmagyar�zza a hiba ok�t
public:
	SDL_error(std::string explanatory = "SDL_ERROR");
	virtual void what(); //Visszaadja a stringet + az SDL_Geterror() f�ggv�nyt
	~SDL_error();
};

