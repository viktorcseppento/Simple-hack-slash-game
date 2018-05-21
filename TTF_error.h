#pragma once

#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <iostream>
#include <string>

class TTF_error :
	public std::exception
{
	std::string explanatory; //Magyarázóstring
public:
	TTF_error(std::string explanatory = "TTF_ERROR");
	virtual void what(); //Elmagyarázza a hiba okát
	~TTF_error();
};
