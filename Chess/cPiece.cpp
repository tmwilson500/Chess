#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>

using namespace sf;

struct move
{
	int startI, startJ, endI, endJ;
	cPiece& movPiece2; //The second piece that was moved (used only for castle moves)
	cPiece& capPiece;  //The piece that was captured (nullptr if move did not result in capture)
};

class cPiece {

public:
	Sprite sprite;
	Texture pTex;
	std::vector<move> moves;
	bool draw = false;
	int ID = 0;
	float x = 0;
	float y = 0;
	int player = 0; // 0 if piece belongs to white, 1 if piece belongs to black
};