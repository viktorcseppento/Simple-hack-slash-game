#include "Enemy.h"



Enemy::Enemy(int indexParam, Graphic *graphic, double posX, double posY): Character(graphic, posX, posY)
{
	damage = 2;
	index = indexParam;

	if (g!= NULL) {
		enemyTexture = new Texture(g, "./Resources/enemy.bmp", width, height, 2);
		if (enemyTexture == NULL)
			throw SDL_error("Couldn't create enemy texture");
	}
	else
		enemyTexture = NULL;
	currentTexture = enemyTexture;
}


Enemy::~Enemy()
{
	if (enemyTexture != NULL) {
		delete enemyTexture;
		enemyTexture = NULL;
	}
	g = NULL;
}

int Enemy::getIndex() const
{
	return index;
}

void Enemy::setIndex(int indexParam)
{
	index = indexParam;
}

int Enemy::getDamage() const
{
	return damage;
}

Texture * Enemy::getEnemyTexture() const
{
	return enemyTexture;
}

std::ostream & operator<<(std::ostream & os, const Enemy & e)
{
	os << e.health << " " << e.x << " " << e.y << " " << (int)e.dir;
	return os;
}

std::istream & operator >> (std::istream & is, Enemy & e)
{
	int d;
	is >> e.health >> e.x >> e.y >> d;
	e.dir = Direction(d);
	return is;
}
