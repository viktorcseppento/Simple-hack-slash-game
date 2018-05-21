#include "Block.h"



Block::Block() : leftNeighbor(NULL), rightNeighbor(NULL), upNeighbor(NULL), downNeighbor(NULL), up(-1), right(-1), down(-1), left(-1)
{
	coords = new int[2];
	coords[0] = 0;
	coords[1] = 0;
}

Block::~Block()
{
	delete[] coords;
}

int* Block::getCoords() const
{
	return this->coords;
}

void Block::setCoords(int x, int y)
{
	coords[0] = x;
	coords[1] = y;
}

Block* Block::getLeftNeighbor() const
{
	return leftNeighbor;
}

Block* Block::getDownNeighbor() const
{
	return downNeighbor;
}

Block* Block::getUpNeighbor() const
{
	return upNeighbor;
}

Block* Block::getRightNeighbor() const
{
	return rightNeighbor;
}

void Block::setLeftNeighbor(Block& b) {
	leftNeighbor = &b;
}

void Block::setDownNeighbor(Block& b) {
	downNeighbor = &b;
}

void Block::setUpNeighbor(Block& b) {
	upNeighbor = &b;
}

void Block::setRightNeighbor(Block& b) {
	rightNeighbor = &b;
}

int Block::getUp()const {
	return up;
}

int Block::getRight()const {
	return right;
}

int Block::getDown()const {
	return down;
}

int Block::getLeft()const {
	return left;
}

void Block::setUp(int path) {
	up = path;
}

void Block::setRight(int path) {
	right = path;
}

void Block::setDown(int path) {
	down = path;
}

void Block::setLeft(int path) {
	left = path;
}

void Block::setBounds(int up, int right, int down, int left) {
	this->left = left;
	this->up = up;
	this->down = down;
	this->right = right;
}