#include "Font.h"

Font::Font(Graphic * g, int size): g(g), size(size), textTexture(NULL), width(0), height(0), posX(0), posY(0)
{
	color = { 255, 255, 255 };
	font = TTF_OpenFont("./Resources/font.ttf", size);
	if (font == NULL)
		throw TTF_error("Couldn't load font");
}

Font::~Font()
{
	freeFont();
	TTF_CloseFont(font);
}

void Font::loadText(std::string text) //Egy szövegből csinálunk egy textúrát
{
	posX = 0;
	posY = 0;
	freeFont();
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL)
		throw TTF_error("Couldn't render text surface");
	textTexture = SDL_CreateTextureFromSurface(g->getRenderer(), textSurface);
	if (textTexture == NULL)
		throw TTF_error("Couldn't create texture from rendered text");
	width = textSurface->w;
	height = textSurface->h;
	posX -= width / 2;
	posY -= height / 2;
	SDL_FreeSurface(textSurface);
}

void Font::drawText(std::string text, int posXParam, int posYParam, SDL_Color newColor)
{
	color = newColor;
	loadText(text);
	posX += posXParam;
	posY += posYParam;
	g->renderText(textTexture, posX, posY, width, height);
}

void Font::freeFont()
{
	if (textTexture != NULL) {
		SDL_DestroyTexture(textTexture);
		textTexture = NULL;
	}
}

void Font::setFontSize(int sizeParam)
{
	size = sizeParam;
	TTF_CloseFont(font);
	font = TTF_OpenFont("./Resources/font.ttf", size);
	if (font == NULL)
		throw TTF_error("Couldn't load font");
}
