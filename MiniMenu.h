#pragma once
#include "Menu.h"
#include "Game.h"

class MiniMenu :
	public Menu
{
public:
	MiniMenu(Graphic* graphic = NULL); //Meghívja ősosztályának a konstruktorát
	~MiniMenu();
	void chooseMenu(); //Menüpontok kiválasztásához függvény
};

