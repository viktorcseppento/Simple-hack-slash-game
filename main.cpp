#include <ctime>
#include "Graphic.h"
#include "Map.h"
#include "Texture.h"
#include "Player.h"
#include "Menu.h"
#include "MainMenu.h"
#include "MiniMenu.h"
#include "SDL_error.h"
#include "TTF_error.h"

int main(int argc, char** argv) {
	Graphic G; //Inicializálunk minden grafikus felületet
	srand((unsigned int)time(0)); //Beállítjuk a random generátor
	try {
		if (G.isRunning()) {
			MainMenu M(&G); //Ha sikerült az SDL inicializálása, elkezdjük a programot a főmenü
			M.chooseMenu();
		}
	}
	catch (SDL_error& e) {
		e.what(); //Ha valamilyen SDL-lel kapcsolatos error volt, dob egy kivételt és egy windows messageboxban megjeleníti
	}
	catch (TTF_error& e) {
		e.what(); //Ugyanaz mint előbb, csak ez az SDL-nek a TTF moduljáró vonatkozo kivételeket kapja el
	}
	catch (std::bad_alloc& e) {
		e.what(); //Ha pointerfoglalás nem sikerült, itt kapjuk el
	}
	G.~Graphic(); //Bezárjuk az SDL-t
	return 0;
}