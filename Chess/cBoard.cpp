#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include "cPiece.cpp"


using namespace sf;


class cBoard {
private:
    RenderWindow win;
    RectangleShape squares[8][8];
    Color sColors[2];
    bool sColor = 1;
    float sW;
    float sH;
    cPiece pieces[64];
    int boardMap[8][8] = {  {2, 1, 0, 0, 0, 0, -1, -2},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {5, 1, 0, 0, 0, 0, -1, -5},
                            {6, 1, 0, 0, 0, 0, -1, -6},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {2, 1, 0, 0, 0, 0, -1, -2} };

    void drawPieces() {
        for (int i = 0; i < 64; i++) {
            if (pieces[i].draw)
                win.draw(pieces[i].sprite);
        }
    }

    void scalePieces() {
        for (int i = 0; i < 64; i++) {
            float pScaleX = sW / pieces[i].sprite.getGlobalBounds().width;
            float pScaleY = sH / pieces[i].sprite.getGlobalBounds().height;
            pieces[i].sprite.setScale(Vector2f(pScaleX, pScaleY));
            pieces[i].sprite.setPosition(Vector2f(pieces[i].x, pieces[i].y));
        }
    }

public:
    
    cBoard(float width, float height, const int boardTheme[2][3], std::map<int, std::string> pieceTheme) {
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
        int index = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                pieces[index].ID = boardMap[i][j];
                pieces[index].x = sW*i;
                pieces[index].y = sH * j;
                std::cout << "piece[" << index << "] ID: " << pieces[index].ID << "\n";
                if (pieces[index].ID != 0) {
                    if (!pieces[index].pTex.loadFromFile(pieceTheme[pieces[index].ID]))
                        throw "could not load texture";
                    pieces[index].sprite.setTexture(pieces[index].pTex);
                    pieces[index].draw = true;
                }
                index++;
            }
        }
        scalePieces();
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
        drawPieces();
        
        win.display();
        return true;
    }
};



