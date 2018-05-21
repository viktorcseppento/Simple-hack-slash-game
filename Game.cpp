#include "Game.h"

Game::Game(Graphic* graphicparam, Player *playerparam, Enemy **enemiesparam, int enemyNumberparam,  Map* mapparam, int scoreparam, int waveCounterparam, unsigned waveTimerParam) :
	graphic(graphicparam), player(playerparam), enemies(enemiesparam), enemyNumber(enemyNumberparam), map(mapparam), score(scoreparam), miniMenu(NULL), font(NULL), waveCounter(waveCounterparam), waveTimer(waveTimerParam)
{
	if (waveTimer == 0)
		waveTimer = SDL_GetTicks() - 10000;

	if (graphic != NULL) {
		if (player == NULL) {
			player = new Player(graphic);
		}
		miniMenu = new MiniMenu(graphic);
		font = new Font(graphic, 40);
		if (map == NULL) {
			map = new Map(graphic);
		}
	}
}


Game::~Game()
{
	delete player;
	delete miniMenu;
	delete font;
	delete map;
	int oldEnemyNumber = enemyNumber;
	for (int i = 0; i < oldEnemyNumber; i++)
		removeEnemy();
}

Graphic* Game::getGraphic() const
{
	return graphic;
}

Player* Game::getPlayer() const
{
	return player;
}

Enemy ** Game::getEnemies() const
{
	return enemies;
}

MiniMenu* Game::getMiniMenu() const
{
	return miniMenu;
}

Map* Game::getMap() const
{
	return map;
}

int Game::getEnemyNumber() const
{
	return enemyNumber;
}

int Game::getWaveCounter() const
{
	return waveCounter;
}

unsigned Game::getWaveTimer() const
{
	return waveTimer;
}

void Game::start()
{
	const double FPS = 60.0;
	unsigned mainTimer = SDL_GetTicks(); //Ahhoz kell, hogy másodpercenként 60-szor frissítse a képernyőt
	unsigned enemyAttackTimer = SDL_GetTicks(); //Csak egy megadott időközönként támadjanak az ellenségek
	SDL_Event ev; //Event deklarálása
	int pressingMoment[5] = {0, 0, 0, 0, 1}; //Gomblenyomások (nyilak) idöpontja ( ha el van engedve, =0)
	Direction lastPressed = Direction::NONE; //Az utoljára lenyomott gomb az elején még semmi
	bool alreadyAttacked = false; //Ha már támadtunk akkor valamennyi ideig nem tudunk mégegyszer
	SDL_RenderClear(graphic->getRenderer());
	SDL_RenderPresent(graphic->getRenderer()); //Képernyőről mindent kitörlünk és frissítjük
	while (1) {
		if ((SDL_GetTicks() - mainTimer) / 1000.0 >= 1 / FPS) {
			spawnWaves(); //Az ellenség-hullámokat elindítjuk
			drawEverything(); //Ellenségeket, játékost, térképet, szövegeket kirajzoljuk
			if (SDL_GetTicks() - player->getAttackingTimer() > 100 && player->getCurrentTexture() == player->getAttackingTexture()) { //Ha támadtunk, egy idő után visszaállítjuk a textúrát
				player->setToStandingTexture();
			}

			while (SDL_PollEvent(&ev) != 0) { //Eventek kezelése
				if (ev.type == SDL_QUIT) {
					throw true;
				}
				if (ev.type == SDL_KEYDOWN) {
					if (ev.key.keysym.sym == SDLK_ESCAPE) {
						for (int i = 0; i < 4; i++)
							pressingMoment[i] = 0;
						unsigned remnant = SDL_GetTicks() - waveTimer; //Ha escape-pel kilépünk a menübe jegyezze meg a jelenlegi timerek állását
						try {
							miniMenu->chooseMenu();
							waveTimer = SDL_GetTicks() - remnant;
						}
						catch (int e) { //Kilépés, játékba visszalépés szabályozása
							if (e == 0)
								throw true;
							else if (e == 1)
								throw false;
							else if (e == 3)
								saveGame();
						}
					}
				}
				lastPressed = keyboardEventHandling(ev, pressingMoment, alreadyAttacked); //Ütést és irányításokat kezelő függvény
			}
			if (legitPosition(player, lastPressed, true)) //Csak akkor tudunk mozogni, ha nem falba vagy ellenségbe kötnénk ki
			{
				if (lastPressed != Direction::NONE) {
					player->alternateMoving(); //A mozgásnak a textúráját betöltjük, vagy visszaállítjuk az állót
					player->move(lastPressed); //Mozgunk
				}
			}
			else 
			{
				player->setDir(lastPressed);
			}
			enemyMotion();
			enemiesAttack(enemyAttackTimer); //Ellenségek is mozognak és ütnek
			if (!player->isAlive()) { //Ha meghaltunk, vége a játéknak
				gameOver();
				throw false;
			}
			mainTimer = SDL_GetTicks();
		}
	}
}

Direction Game::keyboardEventHandling(SDL_Event & ev, int pressingMoment[5], bool& alreadyAttacked)
{
	if (ev.type == SDL_KEYDOWN) {
		if (ev.key.keysym.sym == SDLK_UP && pressingMoment[(int)Direction::UP] == 0)
			pressingMoment[(int)Direction::UP] = SDL_GetTicks();
		else if (ev.key.keysym.sym == SDLK_RIGHT && pressingMoment[(int)Direction::RIGHT] == 0) {
			pressingMoment[(int)Direction::RIGHT] = SDL_GetTicks();
		}
		else if (ev.key.keysym.sym == SDLK_DOWN && pressingMoment[(int)Direction::DOWN] == 0)
			pressingMoment[(int)Direction::DOWN] = SDL_GetTicks();
		else if (ev.key.keysym.sym == SDLK_LEFT && pressingMoment[(int)Direction::LEFT] == 0)
			pressingMoment[(int)Direction::LEFT] = SDL_GetTicks();
		else if (ev.key.keysym.sym == SDLK_SPACE){
			if (alreadyAttacked == false){ //Ha egy ideje nem támadtunk, támadhatunk
				alreadyAttacked = true;
				player->setToAttackingTexture();
				player->resetAttackingTimer();
				attackFrontEnemy();
			}
		}
	}
	if (ev.type == SDL_KEYUP) { //Ha elengedünk egy billenytyűt, akkor betesszük 0-ra, a legkorábbira az gomblenyomás pillanatát
		if (ev.key.keysym.sym == SDLK_UP)
			pressingMoment[(int)Direction::UP] = 0;
		else if (ev.key.keysym.sym == SDLK_RIGHT)
			pressingMoment[(int)Direction::RIGHT] = 0;
		else if (ev.key.keysym.sym == SDLK_DOWN)
			pressingMoment[(int)Direction::DOWN] = 0;
		else if (ev.key.keysym.sym == SDLK_LEFT)
			pressingMoment[(int)Direction::LEFT] = 0;
		else if (ev.key.keysym.sym == SDLK_SPACE)
			alreadyAttacked = false;
	}

	Direction lastPressed = Direction::NONE; //utoljára lenyomott gomb, a semmire inicializálva
	for (int i = 0; i < 5; i++) {
		if (pressingMoment[(int)lastPressed] < pressingMoment[i]) //Megkeresi a legutoljára lenyomott gombot
			lastPressed = (Direction)i; //Egy intet átkonvertál Direction osztállyá
	}
	return lastPressed;
}

void Game::enemyMotion() const
{
	for (int i = 0; i < enemyNumber; i++) { //Minden ellenségen végigmegy
		Enemy *currentEnemy = enemies[i];
		Direction whereIsthePlayer; //Milyen irányban van a játékos
		if (abs(currentEnemy->getX() - player->getX()) > abs(currentEnemy->getY() - player->getY())) { //Amelyik irányban a legmesszebb van a játékos, arra megy
			if (currentEnemy->getX() > player->getX())
				whereIsthePlayer = Direction::LEFT;
			else
				whereIsthePlayer = Direction::RIGHT;
		}
		else
		{
			if (currentEnemy->getY() > player->getY())
				whereIsthePlayer = Direction::UP;
			else
				whereIsthePlayer = Direction::DOWN;
		}
		if (legitPosition(currentEnemy, whereIsthePlayer, false))
			currentEnemy->move(whereIsthePlayer); //Megadott irányba mozog
	}
}

void Game::enemiesAttack(unsigned& enemyAttackTimer) const
{
	if (SDL_GetTicks() - enemyAttackTimer > 300) { //Csak egy megadott időközönként ütnek
		int enemiesNear = collideWithEnemies((int)player->getX() - 1, (int)player->getY() - 1, 26, 26); //Megnézi hány ellenség van körülöttünk, annyian ütnek
		for (int i = 0; i < enemiesNear; i++) {
			enemies[0]->attack(*player); //Mindegy melyik ellenség üti, ugyanannyit sebeznek
		}
		enemyAttackTimer = SDL_GetTicks(); //Ha ütöttek, reseteli a timert
	}
}

void Game::drawEverything() const
{
	SDL_RenderClear(graphic->getRenderer());
	map->drawMap(); //Pálya kirajzolása
	player->drawCharacter(); //Játékos kirajzolása
	for (int i = 0; i < enemyNumber; i++)
		enemies[i]->drawCharacter(); //Ellenségek kirajzolása
	drawTexts(); //Szövegek rajzolása
	SDL_RenderPresent(graphic->getRenderer()); //Egész ablak frissítése
}

void Game::drawTexts() const //Összesen 3 db szöveget írunk ki: Pontszám, életerőnk, aktuális ellenség életereje
{
	SDL_Color color = { 255, 255, 255 };
	std::string scoreString = "Score: ";
	std::string playerHealthString = "Health: ";
	std::string enemyHealthString = "Enemy: ";
	std::string temp;
	char buffer[10];

    itoa(score, buffer, 10);
	temp = buffer;
	scoreString += temp;
	itoa(player->getHealth(), buffer, 10); //Int típusból stringet alkotunk
	temp = buffer;
	playerHealthString += temp;
	itoa(player->getMaxHealth(), buffer, 10);
	temp = buffer;
	playerHealthString += "/" + temp;
	int enemyIndex = getFrontEnemy();
	if (enemyIndex >= 0) {
		Enemy* currentEnemy = enemies[enemyIndex];
		itoa(currentEnemy->getHealth(), buffer, 10);
		temp = buffer;
		enemyHealthString += temp;
		itoa(currentEnemy->getMaxHealth(), buffer, 10);
		temp = buffer;
		enemyHealthString += "/" + temp;
		color = { 255, Uint8(255 * currentEnemy->getHealth() / currentEnemy->getMaxHealth()), Uint8(255 * currentEnemy->getHealth() / currentEnemy->getMaxHealth()) };
		font->drawText(enemyHealthString, (int)(graphic->getScreenWidth()*0.87), (int)(graphic->getScreenHeight()*0.7), color);
	}
	color = { 255, 255, 255 };
	font->drawText(scoreString, (int)(graphic->getScreenWidth()*0.85), (int)(graphic->getScreenHeight()*0.1), color);
	color = { 255, Uint8(255 * player->getHealth() / player->getMaxHealth()), Uint8(255 * player->getHealth() / player->getMaxHealth()) }; //Szín beállítás (minél jobban haldoklunk annál pirosabb)
	font->drawText(playerHealthString, (int)(graphic->getScreenWidth()*0.87), (int)(graphic->getScreenHeight()*0.4), color);

}

void Game::spawnWaves() //Megadott időközönként jönnek a hullámok, egyre több ellenséggel (1,2,3,4,5,...)
{
	if (SDL_GetTicks() - waveTimer > 10000){
		for (int i = 0; i < waveCounter; i++)
			addEnemy();
	waveCounter++;
	waveTimer = SDL_GetTicks();
	}
}

void Game::addEnemy()
{
	int posX, posY; // 24 és 600-24-24 közti számok, mert a széle biztos fal és a textúra pozíciója a bal fölsö sarkának a pozíciója
	Enemy** temp = new Enemy*[enemyNumber + 1];
	do {
		posX = rand() % (600 - 24 - 24 - 24) + 24;
		posY = rand() % (600 - 24 - 24 - 24) + 24;
	} while (collideWithPlayer(posX, posY, 24, 24) || collideWithWall(posX, posY, 24, 24) || collideWithEnemies(posX, posY, 24, 24) != 0); //Addig ad egy random pozíciót az új ellenségnek, amíg nem ütközik fallal, játékossal, vagy másik ellenséggel

	for (int i = 0; i < enemyNumber; i++)
		temp[i] = enemies[i];
	temp[enemyNumber] = new Enemy(enemyNumber, graphic, (double)posX, (double)posY);
	delete[] enemies;
	enemies = temp;
	enemyNumber++;
}

void Game::removeEnemy(int enemyIndex)
{
	if (enemyIndex == -2) //Ha -2-t kap a függvény, nem csinál semmit, mivel nem létezik olyan ellenség
		return;
	if (enemyIndex == -1 || enemyIndex > enemyNumber-1) //Ha -1-et, az alapértelmezett paramétert kapja, akkor az utolsóra állítja az indexet
		enemyIndex = enemyNumber - 1;
	if (enemyNumber != 1) {
		Enemy **temp = new Enemy*[enemyNumber - 1];
		for (int i = 0; i < enemyIndex; i++)
			temp[i] = enemies[i];
		delete enemies[enemyIndex];
		for (int i = enemyIndex + 1; i < enemyNumber; i++) {
			temp[i - 1] = enemies[i];
		}
		delete[] enemies;
		enemies = temp;
		if (enemyIndex != enemyNumber - 1) {
			for (int i = 0; i < enemyNumber - 1; i++) //újraindexelés
				enemies[i]->setIndex(i);
		}
	}
	else { //Ha csak egy ellenség van, akkor az egész pointertömböt is törli
		delete enemies[0];
		delete[] enemies;
		enemies = NULL;
	}
	enemyNumber--;
}

bool Game::legitPosition(Character*const& ch, Direction d, bool isPlayer) const //Megkapja, hogy játékosra, vagy ellenségre nézzük-e meg
{
	double newXPos = ch->getX(), newYPos = ch->getY(); //Megnézi, hol lenne a karakter
	switch (d) {
	case Direction::UP: //Új pozíció kiszámolása
		newYPos -= ch->getSpeed();
		break;
	case Direction::RIGHT:
		newXPos += ch->getSpeed();
		break;
	case Direction::DOWN:
		newYPos += ch->getSpeed();
		break;
	case Direction::LEFT:
		newXPos -= ch->getSpeed();
		break;
	}
	if (isPlayer) { //Ha játékosra nézzük, nem ütközhet fallal, és ellenségekkel
		if (!collideWithWall((int)newXPos, (int)newYPos, ch->getWidth(), ch->getHeight()) && collideWithEnemies((int)newXPos, (int)newYPos, ch->getWidth(), ch->getHeight()) == 0)
			return true;
		else
			return false;
	}
	else { //Ha ellenségre nézzük, nem ütközhet fallal, a játékossal, és másik ellenséggel (magával igen)
		if (!collideWithWall((int)newXPos, (int)newYPos, ch->getWidth(), ch->getHeight()) && collideWithEnemies((int)newXPos, (int)newYPos, ch->getWidth(), ch->getHeight()) == 1 && !collideWithPlayer((int)newXPos, (int)newYPos, ch->getWidth(), ch->getHeight()))
			return true;
		else
			return false;
	}
}

bool Game::collideWithPlayer(int posX, int posY, int width, int height) const //Fő elv, hogy a textúrák négyzet alakúak és koordiátjuk a bal fölső sarkuknak a pozíciója
{
	int playerX = (int)player->getX();
	int playerY = (int)player->getY();
	int playerWidth = player->getWidth();
	int playerHeight = player->getHeight();
	if ((posX >= playerX && posX <= playerX + playerWidth) || (posX + width >= playerX && posX + width <= playerX + playerWidth))
		if ((posY >= playerY && posY <= playerY + playerHeight) || (posY + height >= playerY && posY + height <= playerY + playerHeight))
			return true;
	return false;
}

bool Game::collideWithWall(int posX, int posY, int width, int height) const
{
	int** blueprint = map->getBlueprint();
	int wallWidth = map->getWallTexture()->getWidth();
	int wallHeight = map->getWallTexture()->getHeight();
	for (int i = 0; i < 25; i++) 
		for (int j = 0; j < 25; j++) {
			if (blueprint[i][j] == 0) { //ha fal van ott
				int wallX = j*wallWidth;
				int wallY = i*wallHeight;
				if ((posX >= wallX && posX <= wallX + wallWidth) || (posX + width >= wallX && posX + width <= wallX + wallWidth))
					if ((posY >= wallY && posY <= wallY + wallHeight) || (posY + height >= wallY && posY + height <= wallY + wallHeight))
						return true;
			}
		}
	return false;
}

int Game::collideWithEnemies(int posX, int posY, int width, int height) const
{
	int count = 0;
	for (int i = 0; i < enemyNumber; i++) {
		int enemyWidth = enemies[i]->getWidth();
		int enemyHeight = enemies[i]->getHeight();
		int enemyX = (int)enemies[i]->getX();
		int enemyY = (int)enemies[i]->getY();
		if ((posX >= enemyX && posX <= enemyX + enemyWidth) || (posX + width >= enemyX && posX + width <= enemyX + enemyWidth))
			if ((posY >= enemyY && posY <= enemyY + enemyHeight) || (posY + height >= enemyY && posY + height <= enemyY + enemyHeight))
				count++;
	}
	return count;
}

bool Game::collideWithSpecificEnemy(int enemyIndex, int posX, int posY, int width, int height) const
{
	int i = enemyIndex;
	int enemyWidth = enemies[i]->getWidth();
	int enemyHeight = enemies[i]->getHeight();
	int enemyX = (int)enemies[i]->getX();
	int enemyY = (int)enemies[i]->getY();
	if ((posX >= enemyX && posX <= enemyX + enemyWidth) || (posX + width >= enemyX && posX + width <= enemyX + enemyWidth))
		if ((posY >= enemyY && posY <= enemyY + enemyHeight) || (posY + height >= enemyY && posY + height <= enemyY + enemyHeight))
			return true;
	return false;
}

int Game::enemyIndexAtPosition(int posX, int posY, int width, int height) const
{
	for (int i = 0; i < enemyNumber; i++) { //Megnézi, melyik ellenséggel ütközik a megadott hely
		if (collideWithSpecificEnemy(i, posX, posY, width, height))
			return i;
	}
	return -2;
}

void Game::attackFrontEnemy()
{
	int enemyIndex = getFrontEnemy(); //Megnézzük melyik ellenség van előttünk
	if (enemyIndex >= 0) {
		player->attack(*enemies[enemyIndex]); //Megtámadjuk
		if (enemies[enemyIndex]->isAlive() == false) { //Ha halott, eltüntejük, és kapunk pontot
			removeEnemy(enemyIndex);
			score++;
		}
	}
}

int Game::getFrontEnemy() const
{
	Direction facingDirection = player->getDir();
	int playerX = (int)player->getX();
	int playerY = (int)player->getY();
	int playerWidth = player->getWidth();
	int playerHeight = player->getHeight();
	int enemyIndex = -2;
	if (facingDirection == Direction::UP)
		enemyIndex = enemyIndexAtPosition(playerX + playerWidth / 2, playerY - playerHeight / 2, playerWidth / 2, playerHeight / 2);
	if (facingDirection == Direction::RIGHT)
		enemyIndex = enemyIndexAtPosition(playerX + playerWidth, playerY + playerHeight / 2, playerWidth / 2, playerHeight / 2);
	if (facingDirection == Direction::DOWN)
		enemyIndex = enemyIndexAtPosition(playerX, playerY + playerHeight, playerWidth / 2, playerHeight / 2);
	if (facingDirection == Direction::LEFT)
		enemyIndex = enemyIndexAtPosition(playerX - playerWidth / 2, playerY, playerWidth / 2, playerHeight / 2);
	return enemyIndex;
}

void Game::gameOver() const //Ha meghaltunk, kiírjuk a Game Over szöveget és a szerzett pontot
{
	std::string over = "Game Over";
	char buffer[10];
	itoa(score, buffer, 10);
	std::string temp(buffer);
	std::string scoreString = "Score: " + temp;
	int gameOverTimer = SDL_GetTicks();
	SDL_RenderClear(graphic->getRenderer());
	font->setFontSize(150);
	font->drawText(over, (int)(graphic->getScreenWidth() / 2), (int)(graphic->getScreenHeight() * 0.4), { 255,0,0 });
	font->setFontSize(120);
	font->drawText(scoreString, (int)(graphic->getScreenWidth() / 2), (int)(graphic->getScreenHeight() * 0.7), { 255,0,0 });
	SDL_RenderPresent(graphic->getRenderer());
	while (1) {
		if (SDL_GetTicks() - gameOverTimer > 3000) //3 másodpercig van a képernyőn
			break;
	}
}

void Game::saveGame() const //Mindent kiírunk egy szöveges fájlba (binárisba jobb lenne, de amikor írtam még nem tudtam)
{
	std::ofstream file;
	file.open("./Resources/savegame.txt", std::ios::out);
	if (!file.good())
		throw std::ios_base::failure("Couldn't open savegame.txt"); //Ha nem sikerült megnyitni, dobunk egy hibát
	unsigned remnant = SDL_GetTicks() - waveTimer;
	file << *player<<"\n"<<score<<" "<<enemyNumber<<" "<<waveCounter<<" "<<remnant<<"\n";
	for (int i = 0; i < enemyNumber; i++)
		file << *enemies[i] << "\n";
	file << *map;
	file.close();
}

