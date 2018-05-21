#pragma once

#include "Character.h"

class Enemy :
	public Character
{
	int index; //Tömbben lesznek az ellenségek és egy indexszel külömböztetjük meg őket
	Texture *enemyTexture; //Ellenség textúrája (mindig ez lesz a currentTexture)
public:
	Enemy(int indexParam, Graphic* graphic = NULL, double posX = 0, double posY = 0);
	~Enemy();
	int getIndex() const;
	void setIndex(int indexParam); //Beállítja az indexet
	int getDamage() const;
	Texture* getEnemyTexture() const;
	friend std::ostream& operator<<(std::ostream& os, const Enemy& e);
	friend std::istream& operator >> (std::istream& is, Enemy& e); //Kíír, beolvas
};

