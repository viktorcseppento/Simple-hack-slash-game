#include "SDL_error.h"



SDL_error::SDL_error(std::string str)
{
	explanatory = str;
}


void SDL_error::what()
{
	std::string full = explanatory + " " + SDL_GetError();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_ERROR", full.c_str(), NULL);
}

SDL_error::~SDL_error()
{
}
