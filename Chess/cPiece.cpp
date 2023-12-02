#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>

using namespace sf;

class cPiece {
private:
	Sprite cP;
	Texture pTex;
	bool draw = false;
	int ID;
	int x;
	int y;

public:
	cPiece(int pieceID) {
		ID = pieceID;

	}
};