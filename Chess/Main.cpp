#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <map>
#include "cBoard.cpp"


using namespace sf;


//const std::map<std::string, std::string> pieceTheme1 = { {"bb","Textures/bb.png"},
//                                                         {"bkg","Textures/bkg.png"},
//                                                         {"bkn","Textures/bkn.png"},
//                                                         {"bp","Textures/bp.png"}, 
//                                                         {"bq","Textures/bq.png"}, 
//                                                         {"br","Textures/br.png"}, 
//                                                         {"wb","Textures/wb.png"}, 
//                                                         {"wkg","Textures/wkg.png"}, 
//                                                         {"wkn","Textures/wkn.png"}, 
//                                                         {"wp","Textures/wp.png"}, 
//                                                         {"wq","Textures/wq.png"}, 
//                                                         {"wr","Textures/wr.png"}};
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
    cBoard board(1000.f, 1000.f, boardTheme1, pieceTheme1);
    while (board.Update()) {

    }
    return 0;

    // Create Pieces
    /*Texture wpTex;
    Sprite wp;

    if (!wpTex.loadFromFile(theme1.at("bb")))
        throw "could not load wp.png";

    wp.setTexture(wpTex);
    wp.setScale(Vector2f(1.5f, 1.5f));*/


    
    // GAME LOOP
    //while (window.isOpen())
    //{
    //    // DRAW PIECES
    //    for (int i = 0; i < 8; i++)
    //    {
    //        for (int j = 0; j < 8; j++) {
    //            window.draw(wp);
    //            wp.setPosition(Vector2f(squares[i][j].getSize().x * i, squares[i][j].getSize().y * j));
    //        }
    //    }
    //    window.draw(wp);

    //    window.display();
    //}

    //return 0;
}