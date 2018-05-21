#include "Menu.h"
#include <iostream>


Menu::Menu(Graphic* graphic) : g(graphic), menuTexture(NULL)
{
}


Menu::~Menu()
{
	if (menuTexture != NULL) {
		delete menuTexture;
		menuTexture = NULL;
	}
	g = NULL;
}

Graphic * Menu::getGraphic() const
{
	return g;
}

Texture * Menu::getMenuTexture() const
{
	return menuTexture;
}
