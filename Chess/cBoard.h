#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "cPiece.h"
#include <cstdlib>


using namespace sf;

struct cMove
{
    int startX;
    int startY;
    int endX;
    int endY;
    cPiece* movPiece;  //The primary piece that was moved
    cPiece* capPiece;  //The piece that was captured (nullptr if move did not result in capture)
    cPiece* movPiece2; //The second piece that was moved (used only for castle moves)
    
    //Constructor to initialize pointer members
    cMove(cPiece* primary = nullptr, int sX = 0, int sY = 0, int eX = 0, int eY = 0, cPiece* cap = nullptr, cPiece* secondary = nullptr) 
        : movPiece(primary), startX(sX), startY(sY), endX(eX), endY(eY), capPiece(cap), movPiece2(secondary) {}
};

class cBoard {
public:

    cBoard(float width, float height, const int boardTheme[2][3], std::map<int, std::string> pieceTheme);
    ~cBoard();

    bool Update();

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
    cPiece* pieces[64]; 
    std::vector<cMove*> moveHist;
    int boardMap[8][8] = { {2, 1, 0, 0, 0, 0, -1, -2},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {5, 1, 0, 0, 0, 0, -1, -5},
                            {6, 1, 0, 0, 0, 0, -1, -6},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {2, 1, 0, 0, 0, 0, -1, -2} };

    void drawSquares();

    void drawPieces();

    void scalePieces();

    // True if square with coordinates [sqI][sqJ] contains a drawn piece, otherwise false
    // NOTE: prefer isOccupied() over getPiece() when access to cPiece object attributes is not strictly necessary
    bool isOccupied(int sqI, int sqJ);


    // If square with coordinates [sqI][sqJ] contains a drawn piece, returns a pointer to that piece.
    // Otherwise, returns nullptr
    // NOTE: use only when access to cPiece object attributes is necessary - otherwise use isOccupied()
    cPiece* getPiece(int sqI, int sqJ);

    bool doMove2(cPiece* piece, int targetI, int targetJ);

    void printHist();

    // un-does the last move, removing it from the moveHist vector and returning all pieces to their 
    // original position before that move
    // Returns true if move could be un-done, otherwise returns false
    bool unDoMove();

    // Takes a pointer to a cPiece object and attempts to move it to square at coordinates [targetI][targetJ]
    // If target square contains enemy piece, capture it
    // Return true if move was executed successfully, otherwise return false
    // NOTE: does not fully check legality of move - always check legality using legalMove() first
    bool doMove(cPiece* piece, int targetI, int targetJ);

    //Checks if king belonging to player is in check
    //If player's king is in check, return true, otherwise return false
    bool checkCheck(int player);

    // Checks if moving cPiece object 'piece' to square with coords [newSqI][newSqJ] is legal
    // Returns true if move is legal, otherwise returns false
    // NOTE: does not execute move or modify any pieces - use doMove() to execute move after checking legality
    bool legalMove(cPiece piece, int newSqI, int newSqJ);
};



