#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>

using namespace sf;

class cPiece {
private:
	
	bool draw = false;
	int ID=0;
	int x = 0;
	int y = 0;

public:
	Sprite sprite;
	Texture pTex;
};