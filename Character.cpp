#include "Character.h"



Character::Character(Graphic* graphic, double posX, double posY) :
	g(graphic), alive(true), maxHealth(100), speed(1), width(24), height(24), x(posX), y(posY), dir(Direction::UP), damage(0)
{
	health = maxHealth;
}


Character::~Character()
{
}

bool Character::isAlive() const
{
	return alive;
}

double Character::getX() const
{
	return x;
}

double Character::getY() const
{
	return y;
}

double Character::getSpeed() const
{
	return speed;
}

int Character::getWidth() const
{
	return width;
}

int Character::getHeight() const
{
	return height;
}

Direction Character::getDir() const
{
	return dir;
}

void Character::setDir(Direction newDirection)
{
	dir = newDirection;
}

Texture * Character::getCurrentTexture() const
{
	return currentTexture;
}

int Character::getHealth() const
{
	return health;
}

void Character::beingDamaged(int damageDealt)
{
	health -= damageDealt;
	if (health <= 0)
		alive = false;
}

const int Character::getMaxHealth() const
{
	return maxHealth;
}

void Character::attack(Character & other) const
{
	other.beingDamaged(damage);
}

void Character::move(Direction newDirection) //Az új irányba mozgatjuk
{
	if (newDirection == Direction::NONE)
		return;
	dir = newDirection;
	switch (dir) {
	case Direction::UP:
		y -= speed;
		break;
	case Direction::RIGHT:
		x += speed;
		break;
	case Direction::DOWN:
		y += speed;
		break;
	case Direction::LEFT:
		x -= speed;
		break;
	}
}

void Character::drawCharacter() const //Megfelelően fordítjuk a karektert és rendereljük az ablkara
{
	int angle;
	switch (dir) {
	case Direction::UP:
		angle = 0;
		break;
	case Direction::RIGHT:
		angle = 90;
		break;
	case Direction::DOWN:
		angle = 180;
		break;
	case Direction::LEFT:
		angle = 270;
		break;
	}
	g->renderTexture(*currentTexture, x, y, angle);
}
