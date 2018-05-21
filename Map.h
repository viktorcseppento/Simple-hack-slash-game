#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Block.h"
#include "Texture.h"

class Texture;

class Graphic;

class Map
{
	int **blueprint; //road: 1, wall: 0; 25x25-ös kétdimenziós tömb
	Texture* road; //Az út textúrája
	Texture* wall; //Fal textúrája
	Graphic *g; //Grafikus osztályra pointer
public:
	Map(Graphic* graphic = NULL, int** blueprintParam = NULL, bool create = true); //Létrehoz
	Map(Map& other);
	~Map();
	void createMap(); //Főfüggvény, ami a többi függvény segítségével létrehoz egy térképet
	void setBlockNeighbours(Block**& blocks); //A Blockok szomszédváltozóit beállítja
	void randomBorders(Block**& blocks); //A Blockoknak, aminek tudja megadja a többi Blocktól függően az oldalait, aminek nem tudja annak random adja
	bool BFS(Block**& blocks); //BFS-sel bejárjuk a kész térképet, hogy összefüggő-e
	bool unique(int actI, int actJ, int arr[25][2]); //Megnézi egy 25 méretű tömbben hogy egy koordináta-pár valóban csak egyszer van benne
	void createBlueprint(Block**& blocks); //Az 5x5-ös blocktömbből 25x25-ös int tömböt hoz létre
	void printBlueprint() const; //Kiírja a konzolra 0-kal és 1-esekkel a térképet
	void printBlockprint(Block**& blocks) const; //Itt a blockokat írja ki rendezetten
	Texture* getRoadTexture() const;
	Texture* getWallTexture() const;
	Graphic * getGraphic() const;
	int **getBlueprint() const;
	void drawMap() const; //A Graphic *g tagváltozóval kirajzolja az SDL ablakba a térképet szépen
	friend std::ostream& operator<<(std::ostream& os, const Map& m);
	friend std::istream& operator >> (std::istream& is, Map& m); //Ezek az operátorokra a fájlba írásakor és fájlból olvasásakor lesz szükséges, ha elmentjük a játékot
};