#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace sf;

class cPiece; //Forward declaration of cPiece class

struct cMove
{
	int startI = 0; 
	int startJ = 0;
	int endI = 0;
	int endJ = 0;
	cPiece* movPiece2 = nullptr; //The second piece that was moved (used only for castle moves)
	cPiece* capPiece = nullptr;  //The piece that was captured (nullptr if move did not result in capture)
};

class cPiece {

public:
	Sprite sprite;
	Texture pTex;
	std::vector<cMove> moveHist;
	bool draw = false;
	int ID = 0;
	float x = 0;
	float y = 0;
	int player = 0; // 0 if piece belongs to white, 1 if piece belongs to black
};