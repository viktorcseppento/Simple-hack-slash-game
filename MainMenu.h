#pragma once
#include "Menu.h"
#include "Game.h"

class MainMenu :
	public Menu
{
	bool quit; //Seg�dv�ltoz�, kil�pj�nk-e a j�t�kb�l
public:
	MainMenu(Graphic* graphic= NULL);
	~MainMenu();
	void chooseMenu();
	void newGame(); //�j j�t�k kezd�se
	void loadGame(); //Mentett j�t�k bet�lt�se
};

