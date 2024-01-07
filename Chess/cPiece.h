#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class cPiece; //Forward declaration of cPiece class

class cPiece {

public:
	Sprite sprite;
	Texture pTex;
	bool draw = false;
	int ID = 0;
	float x = 0;
	float y = 0;
	int player = 0; // 0 if piece belongs to white, 1 if piece belongs to black
	bool startPos = true;
};