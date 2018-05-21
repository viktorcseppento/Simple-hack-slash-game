#include "TTF_error.h"

TTF_error::TTF_error(std::string str)
{
	explanatory = str;
}

void TTF_error::what()
{
	std::string full = explanatory + " " + TTF_GetError();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TTF_ERROR", full.c_str(), NULL);
}

TTF_error::~TTF_error()
{
}
