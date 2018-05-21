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
	Texture *menuTexture; //Menük textúrája
	Graphic *g; //Grafikus objektumra mutató pointer
public:
	Menu(Graphic* graphic = NULL); //Semmi extra
	~Menu();
	virtual void chooseMenu() = 0; //Az SDL eseményeket kezeli, menüpontok szabályozása
	Graphic *getGraphic() const;
	Texture* getMenuTexture() const;
};

