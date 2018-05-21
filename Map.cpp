#include "Map.h"




Map::Map(Graphic* graphic, int** blueprintParam, bool create) : g(graphic), road(NULL), wall(NULL), blueprint(blueprintParam)
{
	if (graphic != NULL) { //Betöltjük a textúrákat, ha nem sikerólt dobjuk a kivételt
		road = new Texture(graphic, "./Resources/road.bmp", 24, 24, 0);
		if (road == NULL)
			throw SDL_error("Couldn't create road texture");
		wall = new Texture(graphic, "./Resources/wall.bmp", 24, 24, 0);
		if (wall == NULL)
			throw SDL_error("Couldn't create wall texture");
		if (create) {
			blueprint = new int*[25];
			for (int i = 0; i < 25; i++) {
				blueprint[i] = new int[25];
			}
			for (int i = 0; i < 25; i++)
				for (int j = 0; j < 25; j++)
					blueprint[i][j] = 0;
			createMap(); //A konstruktor végén létrehozunk egy térképet
		}
	}
}

Map::Map(Map & other) //Másoló konstruktor
{
	g = other.g;
	road = other.road;
	wall = other.wall;
	blueprint = new int*[25];
	for (int i = 0; i < 25; i++) {
		blueprint[i] = new int[25];
		for (int j = 0; j < 25; j++) {
			blueprint[i][j] = other.blueprint[i][j];
		}
	}
}


Map::~Map()
{
	for (int i = 0; i < 25; i++) {
		if (blueprint[i] != NULL) {
			delete[] blueprint[i];
			blueprint[i] = NULL;
		}
	}
	delete[] blueprint;
	if (road != NULL)
		delete road;
	if (wall != NULL)
		delete wall;
	g = NULL;
	road = NULL;
	wall = NULL;
	blueprint = NULL;
}

void Map::createMap()
{
	Block **blocks = new Block*[5]; //Először lefoglal egy 5x5-ös területet Blockonak
	for (int i = 0; i < 5; i++) {
		blocks[i] = new Block[5];
	}
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			blocks[i][j].setCoords(j, i); //Inicializálja a Blockok koordinátáit
	setBlockNeighbours(blocks);
	randomBorders(blocks);
	//block into blueprint
	createBlueprint(blocks);
	for (int i = 0; i < 5; i++)
		delete[] blocks[i];
	delete[] blocks;
}

void Map::setBlockNeighbours(Block **& blocks)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			if (i != 0)
				blocks[i][j].setUpNeighbor(blocks[i - 1][j]);
			if (i != 4)
				blocks[i][j].setDownNeighbor(blocks[i + 1][j]);
			if (j != 0)
				blocks[i][j].setLeftNeighbor(blocks[i][j - 1]);
			if (j != 4)
				blocks[i][j].setRightNeighbor(blocks[i][j + 1]);
		}
}

void Map::randomBorders(Block **& blocks)
{
	srand((int)time(NULL));
	bool connected = false;
	do { //Addig ad random határokat a blockoknak, amíg nem lesz összefüggő a térkép
		connected = false;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				blocks[i][j].setUp(rand() % 2);
				blocks[i][j].setRight(rand() % 2);
				blocks[i][j].setDown(rand() % 2);
				blocks[i][j].setLeft(rand() % 2);
				if (i == 0)
					blocks[i][j].setUp(0);
				else
					blocks[i][j].setUp(blocks[i - 1][j].getDown());
				if (j == 0)
					blocks[i][j].setLeft(0);
				else
					blocks[i][j].setLeft(blocks[i][j - 1].getRight());
				if (i == 4)
					blocks[i][j].setDown(0);
				if (j == 4)
					blocks[i][j].setRight(0);
			}
		}
		connected = BFS(blocks);
	} while (!connected);
}

bool Map::BFS(Block **& blocks)
{
	int reached[25][2]; //Elért koordináták tömbje (Minden block egy koordináta)
	int completed[25][2]; //Befejezett koordináták tömbje
	int numberReached = 0;
	int numberCompleted = 0; //Elért és befejezett koordináták száma
	int actI, actJ; //Segédváltozók
	for (int i = 0; i < 25; i++) { //Elején minden érték -1
		reached[i][0] = -1;
		reached[i][1] = -1;
		completed[i][0] = -1;
		completed[i][1] = -1;
	}
	actI = reached[0][0] = 0;
	actJ = reached[0][1] = 0;
	numberReached = 1; //Az első blockkal kezdjük, az már el van érve
	for (int k = 0; k < 25; k++) { //25 Block van, ha 25 ciklus alatt nem végzünk, nem összefüggő
		if (blocks[actI][actJ].getUp() == 1) { //Ha van az aktuális block fölött block akkor megnézzük azt, hogy el tudjuk-e érni közvetlenül úton keresztül és benne van-e már az elértek tömbjében
			if (unique(actI - 1, actJ, reached)) {
				reached[numberReached][0] = actI - 1;
				reached[numberReached][1] = actJ;
				numberReached++;
			}
		}
		if (blocks[actI][actJ].getRight() == 1) { //Minden irányba megnzézük
			if (unique(actI, actJ + 1, reached)) {
				reached[numberReached][0] = actI;
				reached[numberReached][1] = actJ + 1;
				numberReached++;
			}
		}
		if (blocks[actI][actJ].getDown() == 1) {
			if (unique(actI + 1, actJ, reached)) {
				reached[numberReached][0] = actI + 1;
				reached[numberReached][1] = actJ;
				numberReached++;
			}
		}
		if (blocks[actI][actJ].getLeft() == 1) {
			if (unique(actI, actJ - 1, reached)) {
				reached[numberReached][0] = actI;
				reached[numberReached][1] = actJ - 1;
				numberReached++;
			}
		}
		if (unique(actI, actJ, completed)) { //Ha megnéztük egy block minden irányát akkor befejezett lesz, és ha még nem volt befejezett, akkor betesszük a befejezettek tömbjébe
			completed[numberCompleted][0] = actI;
			completed[numberCompleted][1] = actJ;
			numberCompleted++;
		}
		if (numberReached == 25)
			return true; //Ha elértünk mindent igazzal térünk vissza, összefüggő
		actI = reached[numberCompleted][0];
		actJ = reached[numberCompleted][1];
		if (actI == -1 || actJ == -1) //Ha még vannak -1-ek, akkor nem összefüggő
			return false;
	}
	return false;
}

bool Map::unique(int actI, int actJ, int arr[25][2])
{
	for (int i = 0; i < 25; i++)
		if (arr[i][0] == actI && arr[i][1] == actJ) //végigmegy a halmazon és van már olyan érték akkor hamissal tér vissza
			return false;
	return true;
}

void Map::printBlockprint(Block**& blocks) const {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++)
			std::cout << blocks[i][j].getUp() << " " << blocks[i][j].getRight() << " " << blocks[i][j].getDown() << " " << blocks[i][j].getLeft() << std::endl;
		std::cout << std::endl;
	}
}

Texture* Map::getRoadTexture() const
{
	return road;
}

Texture* Map::getWallTexture() const
{
	return wall;
}

Graphic * Map::getGraphic() const
{
	return g;
}

int ** Map::getBlueprint() const
{
	return blueprint;
}

void Map::drawMap() const //rendereli a textúrákat a képernyőre
{
	for (int i = 0; i < 25; i++)
		for (int j = 0; j < 25; j++) {
			if (blueprint[i][j] == 0) {
				g->renderTexture(*wall, j*wall->getWidth(), i*wall->getHeight(), 0);
			}
			if (blueprint[i][j] == 1) {
				g->renderTexture(*road, j*road->getWidth(), i*road->getHeight(), 0);
			}
		}
}

void Map::createBlueprint(Block **& blocks) //Az 5x5-ös BlockPrintből csinál egy Blueprintet, ami után már nem kell Blockokkal foglalkoznunk többet
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			blueprint[5 * i + 0][5 * j + 0] = 0;
			blueprint[5 * i + 4][5 * j + 0] = 0;
			blueprint[5 * i + 0][5 * j + 4] = 0;
			blueprint[5 * i + 4][5 * j + 4] = 0;
			blueprint[5 * i + 1][5 * j + 0] = blueprint[5 * i + 2][5 * j + 0] = blueprint[5 * i + 3][5 * j + 0] = blocks[i][j].getLeft();
			blueprint[5 * i + 4][5 * j + 1] = blueprint[5 * i + 4][5 * j + 2] = blueprint[5 * i + 4][5 * j + 3] = blocks[i][j].getDown();
			blueprint[5 * i + 1][5 * j + 4] = blueprint[5 * i + 2][5 * j + 4] = blueprint[5 * i + 3][5 * j + 4] = blocks[i][j].getRight();
			blueprint[5 * i + 0][5 * j + 1] = blueprint[5 * i + 0][5 * j + 2] = blueprint[5 * i + 0][5 * j + 3] = blocks[i][j].getUp();
			for (int k = 1; k < 4; k++)
				for (int l = 1; l < 4; l++)
					blueprint[5 * i + k][5 * j + l] = 1;
		}
	}
}

void Map::printBlueprint() const { //Használhattunk volna << operátort is
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			std::cout << blueprint[i][j] << " ";
			if (j % 5 == 4)
				std::cout << " ";
		}
		std::cout << std::endl;
		if (i % 5 == 4)
			std::cout << std::endl;
	}
}

std::ostream & operator<<(std::ostream & os, const Map & m)
{
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			os << m.blueprint[i][j] << " ";
		}
		os << "\n";
	}
	return os;
}

std::istream & operator >> (std::istream & is, Map & m)
{
	int** newBlueprint = new int*[25];
	for (int i = 0; i < 25; i++) {
		newBlueprint[i] = new int[25];
	}
	for (int i = 0; i < 25; i++)
		for (int j = 0; j < 25; j++)
			is >> newBlueprint[i][j];
	m.blueprint = newBlueprint;
	return is;
}
