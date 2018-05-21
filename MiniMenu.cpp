#include "MiniMenu.h"



MiniMenu::MiniMenu(Graphic *graphic): Menu(graphic)
{
	if (graphic != NULL) {
		menuTexture = new Texture(graphic, "./Resources/miniMenu.bmp", g->getScreenWidth(), g->getScreenHeight(), 1);
		if (menuTexture == NULL) {
			throw SDL_error("Couldn't create menuTexture");
		}
	}
}


MiniMenu::~MiniMenu()
{
}

void MiniMenu::chooseMenu()
{
	SDL_Event ev;
	int width = g->getScreenWidth();
	int height = g->getScreenHeight();
	SDL_SetWindowBrightness(g->getWindow(), (float)0.75);
	while (1) {
		g->renderTexture(*menuTexture, 0, 0, 0);
		SDL_RenderPresent(g->getRenderer()); //draw the menu
		while (SDL_PollEvent(&ev) != 0) { //Sarokban az X-szel kilépünk
			if (ev.type == SDL_QUIT) {
				SDL_SetWindowBrightness(g->getWindow(), (float)1.0);
				throw 0; //Quit everything
			}
			if (ev.type == SDL_MOUSEBUTTONDOWN) { //Ha lenyomtuk az egérgombot, megnézzük a pillanatnyi helyét a kurzornak
				if (ev.motion.x > width*0.20 && ev.motion.x < width*0.55) {
					if (ev.motion.y > height*0.15 && ev.motion.y < height*0.25)
					{
						SDL_SetWindowBrightness(g->getWindow(), (float)1.0);
						throw 2; //back to the game
					}
					if (ev.motion.y > height*0.40 && ev.motion.y < height*0.50) {
						SDL_SetWindowBrightness(g->getWindow(), (float)1.0);
						throw 3; //save
					}
					if (ev.motion.y > height*0.66 && ev.motion.y < height*0.76) {
						SDL_SetWindowBrightness(g->getWindow(), (float)1.0);
						throw 1; //to main menu
					}
				}
			}
			if (ev.type == SDL_KEYDOWN) { //Escape-pel visszalépünk a játékba
				if (ev.key.keysym.sym == SDLK_ESCAPE) {
					SDL_SetWindowBrightness(g->getWindow(), (float)1.0);
					throw 2; //back to the game
				}
			}
		}
	}
}
