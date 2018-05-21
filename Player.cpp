#include "Player.h"



Player::Player(Graphic *graphic, double posX, double posY) : Character(graphic, posX, posY)
{
	damage = 20;
	speed = 1.3;
	resetAttackingTimer();
	resetMovingtimer();

	if (g != NULL) {
		standingTexture = new Texture(g, "./Resources/standing.bmp", width, height, 2);
		if (standingTexture == NULL)
			throw SDL_error("Couldn't create standingTexture");
		movingTexture = new Texture(g, "./Resources/moving.bmp", width, height, 2);
		if (movingTexture == NULL)
			throw SDL_error("Couldn't create movingTexture");
		attackingTexture = new Texture(g, "./Resources/attacking.bmp", width, height, 2);
		if (attackingTexture == NULL)
			throw SDL_error("Couldn't create attackingTexture");
	}
	else {
		standingTexture = NULL;
		movingTexture = NULL;
		attackingTexture = NULL;
	}
	currentTexture = standingTexture;
}


Player::~Player()
{
	if (standingTexture != NULL) {
		delete standingTexture;
		standingTexture = NULL;
	}
	if (movingTexture != NULL) {
		delete movingTexture;
		movingTexture = NULL;
	}
	if (attackingTexture != NULL) {
		delete attackingTexture;
		attackingTexture = NULL;
	}
	g = NULL;
}


Texture * Player::getMovingTexture() const
{
	return movingTexture;
}

Texture * Player::getAttackingTexture() const
{
	return attackingTexture;
}

void Player::alternateMoving()
{
	if (SDL_GetTicks() - getMovingTimer() > 300) {
		if (currentTexture == movingTexture)
			currentTexture = standingTexture;
		else
			currentTexture = movingTexture;
		resetMovingtimer();
	}
}

int Player::getAttackingTimer() const
{
	return attackingTimer;
}

int Player::getMovingTimer() const
{
	return movingTimer;
}

void Player::resetAttackingTimer()
{
	attackingTimer = SDL_GetTicks();
}

void Player::resetMovingtimer()
{
	movingTimer = SDL_GetTicks();
}

void Player::setToStandingTexture()
{
	currentTexture = standingTexture;
}

void Player::setToAttackingTexture()
{
	currentTexture = attackingTexture;
}

void Player::setToMovingTexture()
{
	currentTexture = movingTexture;
}

Texture *Player::getStandingTexture() const
{
	return standingTexture;
}

std::ostream & operator<<(std::ostream & os, const Player & p)
{
	os << p.health <<" "<< p.x <<" "<< p.y <<" "<< (int)p.dir;
	return os;
}

std::istream & operator >> (std::istream & is, Player & p)
{
	int d;
	is >> p.health >> p.x >> p.y >> d;
	p.dir = (Direction)d;
	return is;
}
