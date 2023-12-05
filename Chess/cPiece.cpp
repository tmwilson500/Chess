#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>

using namespace sf;

class cPiece {

public:
	Sprite sprite;
	Texture pTex;
	bool draw = false;
	int ID = 0;
	float x = 0;
	float y = 0;
	int player = 0; // 0 if piece belongs to white, 1 if piece belongs to black
};