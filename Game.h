#pragma once

#include <cmath>
#include <cstdlib>

#include "Graphic.h"
#include "Texture.h"
#include "Map.h"
#include "Player.h"
#include "Menu.h"
#include "MiniMenu.h"
#include "Enemy.h"
#include "Font.h"

class MiniMenu;

class Game
{
	Graphic *graphic; //Grafikus objektum
	Player *player; //Játékos
	Enemy **enemies; //Ellenségek tömbje
	MiniMenu *miniMenu; //Escape-pel elérhető menü
	Map *map; //Pálya
	Font* font; //Betűtípus, szövegek
	int enemyNumber; //Ellenségek száma
	int score; //Pontszám
	int waveCounter; //Ellenségek hullámainak száma
	unsigned waveTimer; //Ez határozza meg, hogy mikor jön a következő hullám ellenség
public:
	Game(Graphic* graphicparam = NULL, Player *playerparam = NULL, Enemy **enemiesparam = NULL, int enemyNumberparam = 0, Map* mapparam = NULL, int scoreparam = 0, int waveCounterparam = 1, unsigned waveTimerParam = 0);
	~Game();
	Graphic* getGraphic() const;
	Player* getPlayer() const;
	Enemy** getEnemies() const;
	MiniMenu* getMiniMenu() const;
	Map* getMap() const;
	int getEnemyNumber() const;
	int getWaveCounter() const;
	unsigned getWaveTimer() const;
	void start(); //Elindítja a játékot
	Direction keyboardEventHandling(SDL_Event& ev, int pressingMoment[5], bool& alreadyAttacked); //A leütött billentyűket érzékeli és támad vagy mozog a karakter
	void enemyMotion() const; //Ellenségeket mozgatja
	void enemiesAttack(unsigned& enemyAttackTimer) const; //Az ellenségek ekkor ütnek
	void drawEverything() const; //Mindent, azaz mindent kirajzol
	void drawTexts() const; //Csak a szövegeket rajzolja ki
	void spawnWaves(); //Előhívja az ellenséghullámokat
	void addEnemy(); //Egy ellenséget hozzáad
	void removeEnemy(int enemyIndex = -1); //Ellenséget töröl, ha nincs index akkor a legutolsót törli
	bool legitPosition(Character*const& ch, Direction d, bool isPlayer) const; //Egy karakterpointer-referenciát, irányt kap és megkapja, hogy játékosra vagy ellenségre nézzük-e meg, hogy ütközne-e valamivel
	bool collideWithPlayer(int posX, int posY, int width = 24, int height = 24) const; //Megnézi, hogy egy pozíció ütközik-e a játékossal
	bool collideWithWall(int posX, int posY, int width = 24, int height = 24) const; //Fallal
	int collideWithEnemies(int posX, int posY, int width = 24, int height = 24) const; //Ellenséggel
	bool collideWithSpecificEnemy(int enemyIndex, int posX, int posY, int width = 24, int height = 24) const; //Egy specifikus ellenséggel
	int enemyIndexAtPosition(int posX, int posY, int width = 24, int height = 24) const; //Egy adott pozíción lévő ellenség indexét adja vissza, ha nincs ott ellenség -2-t ad vissza
	void attackFrontEnemy(); //Megtámadjuk az előttünk lévő ellenséget
	int getFrontEnemy() const; //Visszaadja az előttünk lévő ellenség indexét
	void gameOver() const; //Játékot lezáró függvény
	void saveGame() const; //Játékot elmentő függvény
};
