#pragma once

#include "Graphic.h"
#include "Texture.h"

enum class Direction { UP, RIGHT, DOWN, LEFT, NONE };

class Texture;

class Character
{
protected:
	Graphic *g; //SDL-t összefoglaló obejktumra egy pointer
	bool alive; //Életben van-e a karakter
	int health; //Élete
	const int maxHealth; //Maximum élete
	Direction dir; //Aktuális iránya
	double speed; //Sebessége
	int damage; //Sebzése
	Texture* currentTexture; //Jelenlegi textúra (a játékos osztályban több textúra is lesz)
	double x;
	double y; //x és y koordináták a térképen
	int width;
	int height; //Textúra szélessége, magassága
public:
	Character(Graphic* graphic = NULL, double posX = 24.0, double posY = 24.0); //Inicializálja a változókat
	virtual ~Character();
	bool isAlive() const; //Visszaadja, hogy él-e még a karakter
	double getX() const;
	double getY() const;
	double getSpeed() const;
	int getWidth() const;
	int getHeight() const;
	Direction getDir() const;
	void setDir(Direction newDirection);
	Texture* getCurrentTexture() const;
	int getHealth() const;
	void beingDamaged(int damageDealt); //Sebzést kap, ha egy másik karakter megütötte
	const int getMaxHealth() const;
	void attack(Character& other) const; //Megtámad egy másik karaktert
	void move(Direction newDirection); //Mozgatja a karaktert a megadott irányba
	void drawCharacter() const; //Kirajzolja a karaktert a képernyőre
};

