#pragma once

#include <iostream>

	class Block //A térkép 25 darab Block-ból fog felépülni
	{
		int up, right, down, left; //-1: meg nem határozott, 0: fal, 1: út; elmondja hogy a block oldalain milyen textúra van
		Block *leftNeighbor, *rightNeighbor, *upNeighbor, *downNeighbor; //szomszédblockokra mutató pointerek
		int* coords; //2 db 0 és 5 közötti szám; a térképen a koordináták
	public:
		Block();
		~Block();
		int* getCoords() const; //Getterek és setterek következnek
		void setCoords(int x, int y);
		Block* getLeftNeighbor() const;
		Block* getDownNeighbor() const;
		Block* getUpNeighbor() const;
		Block* getRightNeighbor() const;
		void setLeftNeighbor(Block& b);
		void setDownNeighbor(Block& b);
		void setUpNeighbor(Block& b);
		void setRightNeighbor(Block& b);
		int getUp() const;
		int getRight() const;
		int getDown() const;
		int getLeft() const;
		void setUp(int path);
		void setRight(int path);
		void setLeft(int path);
		void setDown(int path);
		void setBounds(int up, int right, int down, int left); //Paraméterként megkapott értékek alapján beállítja a block oldalait
	};