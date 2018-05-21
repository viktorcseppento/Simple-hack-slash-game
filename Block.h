#pragma once

#include <iostream>

	class Block //A t�rk�p 25 darab Block-b�l fog fel�p�lni
	{
		int up, right, down, left; //-1: meg nem hat�rozott, 0: fal, 1: �t; elmondja hogy a block oldalain milyen text�ra van
		Block *leftNeighbor, *rightNeighbor, *upNeighbor, *downNeighbor; //szomsz�dblockokra mutat� pointerek
		int* coords; //2 db 0 �s 5 k�z�tti sz�m; a t�rk�pen a koordin�t�k
	public:
		Block();
		~Block();
		int* getCoords() const; //Getterek �s setterek k�vetkeznek
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
		void setBounds(int up, int right, int down, int left); //Param�terk�nt megkapott �rt�kek alapj�n be�ll�tja a block oldalait
	};