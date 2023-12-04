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
    cPiece pieces[32];


    void drawPiece() {
        
        win.draw(pieces[0].sprite);
        
    }

    void scalePiece() {
        for (int i = 0; i < 1; i++) {
            float pScaleX = sW / pieces[i].sprite.getGlobalBounds().width;
            float pScaleY = sH / pieces[i].sprite.getGlobalBounds().height;
            pieces[i].sprite.setScale(Vector2f(pScaleX, pScaleY));
            pieces[i].sprite.setPosition(Vector2f(0.f, 0.f));


            std::cout << "piece scale x expected: " << pScaleX << "\n";
            std::cout << "piece scale y expected: " << pScaleY << "\n";
            Vector2f pScale = pieces[i].sprite.getScale();
            std::cout << "piece scale x actual : " << pScale.x << "\n";
            std::cout << "piece scale y actual : " << pScale.y << "\n";

            // print global bounts
            FloatRect pBoundsG = pieces[i].sprite.getGlobalBounds();
            std::cout << "global piece width x: " << pBoundsG.width << "\n";
            std::cout << "global piece height y: " << pBoundsG.height << "\n";
            
            // print local bounts
            FloatRect pBoundsL = pieces[i].sprite.getLocalBounds();
            std::cout << "local piece width x: " << pBoundsL.width << "\n";
            std::cout << "local piece height y: " << pBoundsL.height << "\n";
        }
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

                // print global bounds
                FloatRect sBoundsG = squares[i][j].getGlobalBounds();
                std::cout << "global square width x: " << sBoundsG.width << "\n";
                std::cout << "global square height y: " << sBoundsG.height << "\n";

                // print local bounds
                FloatRect sBoundsL = squares[i][j].getLocalBounds();
                std::cout << "local square width x: " << sBoundsL.width << "\n";
                std::cout << "local square height y: " << sBoundsL.height << "\n";
            }
            sColor = !sColor;
        }

        win.create(VideoMode(width, height), "Chess");

        // CREATE PIECES


        for (int i = 0; i < 1; i++) {
            if (!pieces[i].pTex.loadFromFile("Textures/wp.png"))
                throw "could not load wp.png";
            pieces[i].sprite.setTexture(pieces[i].pTex);
            scalePiece();
            /*FloatRect pBounds = pieces[0].sprite.getLocalBounds();
            Vector2f pScale = pieces[0].sprite.getScale();
            std::cout << "piece width x: " << pBounds.width << "\n";
            std::cout << "piece height y: " << pBounds.height << "\n";
            std::cout << "piece scale x: " << pScale.x << "\n";
            std::cout << "piece scale y: " << pScale.y << "\n";*/
        }
        
        

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



