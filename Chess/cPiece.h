#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class cPiece; //Forward declaration of cPiece class

/**
 * @brief Class representing one piece on the chess board
*/
class cPiece {

public:
	Sprite sprite;/**< The underlying SFML Sprite object used to display the chess piece*/
	Texture pTex;/**< The SFML texture for the chess piece*/
	bool draw = false;/**< Boolean value indicating whether the piece shoud be displayed on the board*/
	int ID = 0;/**< The piece ID (indicates which type of piece it is)*/
	float x = 0;/**< The x-coordinate of the piece within the game window*/
	float y = 0;/**< The y-coordinate of the piece within the game window*/
	int player = 0;/**< The player that owns the piece (0 if piece belongs to white, 1 if piece belongs to black)*/
	bool startPos = true;/**< Boolean value indicating whether the piece has moved from its starting position*/
};