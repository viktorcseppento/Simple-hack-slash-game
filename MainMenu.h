#pragma once
#include "Menu.h"
#include "Game.h"

class MainMenu :
	public Menu
{
	bool quit; //Segédváltozó, kilépjünk-e a játékból
public:
	MainMenu(Graphic* graphic= NULL);
	~MainMenu();
	void chooseMenu();
	void newGame(); //Új játék kezdése
	void loadGame(); //Mentett játék betöltése
};

