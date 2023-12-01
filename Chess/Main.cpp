#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <map>
#include "Board.cpp"


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
    RenderWindow window(VideoMode(800, 800), "Chess");
    

    //// Create board
    RectangleShape squares[8][8];
    Color sColors[2];
    bool sColor = 1;
    sColors[0].r = 118;
    sColors[0].g = 150;
    sColors[0].b = 86;
    sColors[1].r = 238;
    sColors[1].g = 238;
    sColors[1].b = 210;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            squares[i][j].setSize(Vector2f(100.f, 100.f));
            squares[i][j].setOutlineThickness(-1.f);
            squares[i][j].setOutlineColor(Color::Black);
            squares[i][j].setPosition(Vector2f(squares[i][j].getSize().x * i, squares[i][j].getSize().y * j));
            squares[i][j].setFillColor(sColors[sColor]);
            sColor = !sColor;
        }
        sColor = !sColor;
    }

    // Create Pieces
    Texture wpTex;
    Sprite wp;

    if (!wpTex.loadFromFile(theme1.at("bb")))
        throw "could not load wp.png";

    wp.setTexture(wpTex);
    wp.setScale(Vector2f(1.5f, 1.5f));


    
    // GAME LOOP
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            // CLOSE WINDOW EVENTS
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
        }

        window.clear(Color::White);

         //DRAW BOARD
         
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
                window.draw(squares[i][j]);
        }


        // DRAW PIECES
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++) {
                window.draw(wp);
                wp.setPosition(Vector2f(squares[i][j].getSize().x * i, squares[i][j].getSize().y * j));
            }
        }
        window.draw(wp);

        window.display();
    }

    return 0;
}