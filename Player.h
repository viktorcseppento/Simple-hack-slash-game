#pragma once

#include "Character.h"

class Player:
	public Character
{
	Texture* standingTexture; //Akkor jelenik meg ez a textúra - ha a játékos egy helyben áll
	Texture* movingTexture; //Ha mozog
	Texture* attackingTexture; //Ha támad
	int attackingTimer; //Számolja az időt, hogy mikor támadt utoljára és egy pici ideig nem üthet mégegyszer
	int movingTimer; //Hasonló; ezek a timerek azért szükségesek, hogy a textúrák szépen legyenek betöltve
public:
	Player(Graphic *graphic = NULL, double posX = 48.0, double posY = 48.0);
	~Player();
	Texture* getStandingTexture() const;
	Texture* getMovingTexture() const;
	Texture* getAttackingTexture() const;
	void alternateMoving(); //A standing és movingTexture-t váltja egymás után
	int getAttackingTimer() const;
	int getMovingTimer() const;
	void resetAttackingTimer(); //Reseteli az időzítőket
	void resetMovingtimer();
	void setToStandingTexture(); //CurrentTexturet beállítják a megfelelő értékekre
	void setToAttackingTexture();
	void setToMovingTexture();
	friend std::ostream& operator<<(std::ostream& os, const Player& p);
	friend std::istream& operator>>(std::istream& is, Player& p); //Kiíró, beolvasó operátorok
};

