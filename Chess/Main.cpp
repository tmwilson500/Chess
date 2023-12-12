#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <map>
#include "cBoard.cpp"


using namespace sf;

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

const int boardTheme1[2][3] = { {118,150,86},
                                {238,238,210} };



int main()
{
    cBoard board(600.f, 600.f, boardTheme1, pieceTheme1);
    while (board.Update()) {

    }
    return 0;
}