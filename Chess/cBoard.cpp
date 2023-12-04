#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include "cPiece.cpp"


using namespace sf;

//int board[8][8] = { {2, 1, 0, 0, 0, 0, -1, -2},
//                    {3, 1, 0, 0, 0, 0, -1, -3},
//                    {4, 1, 0, 0, 0, 0, -1, -4},
//                    {5, 1, 0, 0, 0, 0, -1, -5},
//                    {6, 1, 0, 0, 0, 0, -1, -6},
//                    {4, 1, 0, 0, 0, 0, -1, -4},
//                    {3, 1, 0, 0, 0, 0, -1, -3},
//                    {2, 1, 0, 0, 0, 0, -1, -2} };

class cBoard {
private:
    RenderWindow win;
    RectangleShape squares[8][8];
    Color sColors[2];
    bool sColor = 1;
    float sW;
    float sH;
    cPiece pieces[1];


    void drawPiece() {
        win.draw(pieces[0].sprite);

    }

public:
    
    cBoard(float width, float height, const int boardTheme[2][3]) {
        sW = width/ 8;
        sH = height / 8;

        // Set board color theme
        for (int i = 0; i < 2; i++)
        {
            sColors[i].r = boardTheme[i][0];
            sColors[i].g = boardTheme[i][1];
            sColors[i].b = boardTheme[i][2];
        }


        // Set square size & color
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                squares[i][j].setSize(Vector2f(sW, sH));
                squares[i][j].setOutlineThickness(-1.f);
                squares[i][j].setOutlineColor(Color::Black);
                squares[i][j].setPosition(Vector2f(squares[i][j].getSize().x * i, squares[i][j].getSize().y * j));
                squares[i][j].setFillColor(sColors[sColor]);
                sColor = !sColor;
            }
            sColor = !sColor;
        }

        win.create(VideoMode(width, height), "Chess");

        // CREATE PIECES
        pieces[0].sprite.setPosition(Vector2f(20.f, 20.f));
        pieces[0].sprite.setScale(Vector2f(2.f, 2.f));
        if (!pieces[0].pTex.loadFromFile("Textures/wp.png"))
            throw "could not load wp.png";
        pieces[0].sprite.setTexture(pieces[0].pTex);
        

    }

    bool Update() {
        Event event;
        while (win.pollEvent(event))
        {
            // CLOSE WINDOW EVENTS
            if (event.type == Event::Closed)
            {
                win.close();
                return false;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                win.close();
                return false;
            }
        }

        win.clear(Color::White);

        // Draw squares
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
                win.draw(squares[i][j]);
        }

        // DRAW PIECES
        drawPiece();
        
        win.display();
        return true;
    }
};



