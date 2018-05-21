#include "MainMenu.h"



MainMenu::MainMenu(Graphic *graphic) : Menu(graphic), quit(false)
{
	if (graphic != NULL) {
		menuTexture = new Texture(graphic, "./Resources/mainMenu.bmp", g->getScreenWidth(), g->getScreenHeight(), 0);
		if (menuTexture == NULL)
			throw SDL_error("Couldn't create menuTexture");
	}
}


MainMenu::~MainMenu()
{
}

void MainMenu::chooseMenu()
{
	SDL_Event ev;
	int width = g->getScreenWidth();
	int height = g->getScreenHeight();
	while (!quit) {
		g->renderTexture(*menuTexture, 0, 0, 0);
		SDL_RenderPresent(g->getRenderer()); //Menü kirajzolása
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) {
				quit = true;
			}
			if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (ev.motion.x > width*0.20 && ev.motion.x < width*0.55) {
					if (ev.motion.y > height*0.15 && ev.motion.y < height*0.25)
						newGame();
					if (ev.motion.y > height*0.40 && ev.motion.y < height*0.50)
						loadGame();
					if (ev.motion.y > height*0.66 && ev.motion.y < height*0.76)
						quit = true;
				}
			}
			if (ev.type == SDL_KEYDOWN)
				if (ev.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
		}
	}
}

void MainMenu::newGame()
{
	Game GAME(g);
	try {
		GAME.start();
	}
	catch (bool e) {
		quit = e;
	}
}

void MainMenu::loadGame() //.txt fájlból betöltjük a mentett játékot
{
	std::ifstream file;
	try {
		file.open("./Resources/savegame.txt", std::ios::in);
		if (!file.good())
			throw std::ios_base::failure("Couldn't open savegame.txt");
		Player* player = new Player(g);
		file >> *player;
		int score;
		int enemyNumber;
		int wavecounter;
		unsigned remnant;
		file >> score >> enemyNumber >> wavecounter >> remnant;
		Enemy** enemies = new Enemy*[enemyNumber];
		for (int i = 0; i < enemyNumber; i++) {
			enemies[i] = new Enemy(i, g);
			file >> *enemies[i];
		}
		Map* map = new Map(g, NULL, false);
		file >> *map;
		file.close();
		Game GAME(g, player, enemies, enemyNumber, map, score, wavecounter, SDL_GetTicks() - remnant); //Beolvasott elemekkel indítjuk el a játékot
		try {
			GAME.start();
		}
		catch (bool e) { //Nem jó gyakorlat bool-t elkapni, de ilyen egyszerű programban elfogadható
			quit = e;
		}
	}
	catch (std::ios_base::failure&) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing file", "There is no saved game", NULL); //Windows üzenetdoboz megjelenik
	}
}