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
    int selectIndex = -1;
    bool pieceSelected = false;
    int turn = 0; // 0 for white to move, 1 for black to move
    cPiece pieces[64];
    int boardMap[8][8] = {  {2, 1, 0, 0, 0, 0, -1, -2},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {5, 1, 0, 0, 0, 0, -1, -5},
                            {6, 1, 0, 0, 0, 0, -1, -6},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {2, 1, 0, 0, 0, 0, -1, -2} };

    void drawSquares() {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
                win.draw(squares[i][j]);
        }
    }

    void drawPieces() {
        for (int i = 0; i < 64; i++) {
            if (pieces[i].draw)
                pieces[i].sprite.setPosition(Vector2f(pieces[i].x, pieces[i].y));
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

    bool legalMove(cPiece piece, int newSqI, int newSqJ) {
        int oldSqI = piece.x / sW;
        int oldSqJ = piece.y / sH;
        switch (piece.ID)
        {
        case 1:     //Rules for black pawn
            std::cout << "checking rules for black pawn...\n";

            // Pawn on starting square can move 2 spaces forward only when neither of those spaces is occupied
            if ((oldSqJ == 1) && (newSqI == oldSqI) && (newSqJ == oldSqJ + 2))
                return true;

            // Pawn can always move 1 space forward, except when the destination is occupied
            if ((newSqI == oldSqI) && (newSqJ == oldSqJ + 1))
            {
                for (int i = 0; i < 64; i++) //Check if any piece is already in destination square
                {
                    if (!pieces[i].draw) // Skip check if piece is not drawn
                        continue;
                    int chkSqI = pieces[i].x / sW;
                    int chkSqJ = pieces[i].y / sH;
                    if ((chkSqI == newSqI) && (chkSqJ == newSqJ))
                    {
                        std::cout << "Invalid move: space was occupied by pieces[" << i << "]\n";
                        std::cout << "pieces[" << i << "] has ID: " << pieces[i].ID << "\n";
                        return false;
                    }
                }
                return true;
            }
            else
                std::cout << "Invalid move: no rules implemented for desired move\n";
                return false;
            break;
        case 2:     //Rules for black rook
            return true;
            break;
        case 3:     //Rules for black knight
            return true;
            break;
        case 4:     //Rules for black bishop
            return true;
            break;
        case 5:     //Rules for black queen
            return true;
            break;
        case 6:     //Rules for black king
            return true;
            break;
        case -1:     //Rules for white pawn
            std::cout << "checking rules for white pawn...\n";

            // Pawn on starting square can move 2 spaces forward only when neither of those spaces is occupied
            if ((oldSqJ == 6) && (newSqI == oldSqI) && (newSqJ == oldSqJ - 2))
                return true;

            // Pawn can always move 1 space forward, except when the destination is occupied
            if ((newSqI == oldSqI) && (newSqJ == oldSqJ - 1))
            {
                for (int i = 0; i < 64; i++) //Check if any piece is already in destination square
                {
                    if (!pieces[i].draw) // Skip check if piece is not drawn
                        continue;
                    int chkSqI = pieces[i].x / sW;
                    int chkSqJ = pieces[i].y / sH;
                    if ((chkSqI == newSqI) && (chkSqJ == newSqJ))
                    {
                        std::cout << "Invalid move: space was occupied by pieces[" << i << "]\n";
                        return false;
                    }
                }
                return true;
            }
            else
                std::cout << "Invalid move: no rules implemented for desired move\n";
                return false;
            break;
        case -2:     //Rules for white rook
            return true;
            break;
        case -3:     //Rules for white knight
            return true;
            break;
        case -4:     //Rules for white bishop
            return true;
            break;
        case -5:     //Rules for white queen
            return true;
            break;
        case -6:     //Rules for white king
            return true;
            break;
        default:
            return false;
            break;
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
                if (pieces[index].ID > 0)
                    pieces[index].player = 1;
                else if (pieces[index].ID < 0)
                    pieces[index].player = 0;
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
            switch (event.type)
            {
            case Event::Closed:
                win.close();
                return false;
                break;
            case Event::KeyPressed:
                if (event.key.code == Keyboard::Escape)
                {
                    win.close();
                    return false;
                }
                break;
            case Event::MouseButtonPressed:
                if (((event.mouseButton.button == sf::Mouse::Button::Left) && pieceSelected) && selectIndex >= -1)
                {
                    Vector2f mousePos2 = win.mapPixelToCoords(Mouse::getPosition(win));
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (squares[i][j].getGlobalBounds().contains(mousePos2))
                            {
                                std::cout << "you clicked square: [" << i << "][" << j << "]" << "\n";
                                std::cout << "checking if move was valid for pieces["<<selectIndex<<"]\n";
                                if (legalMove(pieces[selectIndex],i,j))
                                {
                                    std::cout << "move was valid\n";
                                    pieces[selectIndex].x = sW * i;
                                    pieces[selectIndex].y = sH * j;
                                    turn = 1 - turn;
                                }
                                else
                                    std::cout << "Move was not valid\n";
                                pieceSelected = false;
                                selectIndex = -1;
                            }
                        }
                    }
                }
                if ((event.mouseButton.button == sf::Mouse::Button::Left) && !pieceSelected)
                {
                    Vector2f mousePos1 = win.mapPixelToCoords(Mouse::getPosition(win));
                    for (int k = 0; k < 64; k++)
                    {
                        if (pieces[k].sprite.getGlobalBounds().contains(mousePos1))
                        {
                            if (pieces[k].player == turn)
                            {
                                selectIndex = k;
                                pieceSelected = true;
                                std::cout << "you clicked piece ID: " << pieces[selectIndex].ID << "\n";
                            }
                        }
                    }
                }
            }
        }

        win.clear(Color::White);

        // Draw squares
        drawSquares();

        // DRAW PIECES
        drawPieces();
        
        win.display();
        return true;
    }
};



