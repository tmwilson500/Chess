#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <map>
#include "cBoard.h"

/**
* @example
*/
using namespace sf;

// Define a map that correlates piece IDs to the appropriate texture file paths (should be PNG files)
const std::map<int, std::string> pieceTheme1 = { 
    {1, "Textures/bp.png"},
    {2, "Textures/br.png"},
    {3, "Textures/bkn.png"},
    {4, "Textures/bb.png"},
    {5, "Textures/bq.png"},
    {6, "Textures/bkg.png"},
    {-1, "Textures/wp.png"},
    {-2, "Textures/wr.png"},
    {-3, "Textures/wkn.png"},
    {-4, "Textures/wb.png"},
    {-5, "Textures/wq.png"},
    {-6, "Textures/wkg.png"} };

// Define an array of RGB color codes for dark & light squares on the board
// Array should be formatted as: { {dark.r, dark.g, dark.b}, {light.r, light.g, light.b} }
const int boardTheme1[2][3] = { {118,150,86},
                                {238,238,210} };



int main()
{
    // Create an instance of the cBoard class using the desired initial window size (x & y), and board theme and piece theme defined above
    cBoard board(600.f, 600.f, boardTheme1, pieceTheme1);
    
    // Continuously run the Update() function to check for user input until the game is finished
    while (board.Update()) {

    }
    return 0;
}