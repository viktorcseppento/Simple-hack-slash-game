#pragma once
#include "Texture.h"
#include "Map.h"
#include "Player.h"
#include <iostream>

class Texture;
class Graphic;

class Menu
{
protected:
	Texture *menuTexture; //Men�k text�r�ja
	Graphic *g; //Grafikus objektumra mutat� pointer
public:
	Menu(Graphic* graphic = NULL); //Semmi extra
	~Menu();
	virtual void chooseMenu() = 0; //Az SDL esem�nyeket kezeli, men�pontok szab�lyoz�sa
	Graphic *getGraphic() const;
	Texture* getMenuTexture() const;
};

