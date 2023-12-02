#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <map>
#include "cBoard.cpp"


using namespace sf;


const std::map<std::string, std::string> theme1 = { {"bb","Textures/bb.png"},
                                                    {"bkg","Textures/bkg.png"},
                                                    {"bkn","Textures/bkn.png"},
                                                    {"bp","Textures/bp.png"}, 
                                                    {"bq","Textures/bq.png"}, 
                                                    {"br","Textures/br.png"}, 
                                                    {"wb","Textures/wb.png"}, 
                                                    {"wkg","Textures/wkg.png"}, 
                                                    {"wkn","Textures/wkn.png"}, 
                                                    {"wp","Textures/wp.png"}, 
                                                    {"wq","Textures/wq.png"}, 
                                                    {"wr","Textures/wr.png"}};
const std::map<int, std::string> pieceID = { {1, "bp"},
                                             {2, "br"},
                                             {3, "bkn"},
                                             {4, "bb"},
                                             {5, "bq"},
                                             {6, "bkg"},
                                             {-1, "wp"},
                                             {-2, "wr"},
                                             {-3, "wkn"},
                                             {-4, "wb"},
                                             {-5, "wq"},
                                             {-6, "wkg"} };




int main()
{
    cBoard board(600.f, 600.f);
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