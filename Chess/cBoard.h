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
    int castleSide;//-1 for queen side castle, 1 for king side castle, 0 for non-castle moves (default)
    bool wasStart; //True when moved piece originated at its starting position
    cPiece* movPiece;  //The primary piece that was moved
    cPiece* capPiece;  //The piece that was captured (nullptr if move did not result in capture)
    cPiece* movPiece2; //The second piece that was moved (used only for castle moves)
    
    //Constructor to initialize pointer members
    cMove(cPiece* primary = nullptr, int sX = 0, int sY = 0, int eX = 0, int eY = 0, bool start = false, cPiece* cap = nullptr, cPiece* secondary = nullptr, int castle = 0) 
        : movPiece(primary), startX(sX), startY(sY), endX(eX), endY(eY), wasStart(start), capPiece(cap), movPiece2(secondary), castleSide(castle) {}
};

/**
 * @brief A class which represents the chess board & handles all updates to the board state
 * 
 * This class draws & scales the board/pieces, handles click/keyboard events, checks move legality, and updates the locations of pieces
*/
class cBoard {
public:

    /**
     * @brief cBoard class constructor
     *
     * Sets the board scale & color, creates the SFML window, and initializes all chess pieces (cPiece objects) including loading and setting their textures
     *
     * @param width The width of the game window
     * @param height The height of the game window
     * @param boardTheme[2][3] The RGB color codes of dark & light squares on the board, reresented as: { {dark.r, dark.g, dark.b}, {light.r, light.g, light.b} }
     * @param pieceTheme A std::map which correlates the chess piece ID (int) with the path (string) to the texture for that piece (a PNG file)
    */
    cBoard(float width, float height, const int boardTheme[2][3], std::map<int, std::string> pieceTheme);
    
    /**
     * @brief cBoard class destructor
     * 
     * Frees all memory allocated for cPiece objects used by the cBoard class
    */
    ~cBoard();

    /**
     * @brief Handles all MouseButtonPressed/KeyPressed events & updates the board state accordingly
     * 
     * Polls for mouse/keyboard events, checks move legality & updates piece positions when necessary, then re-draws chess board/pieces to the screen
     * 
     * @return always true
    */
    bool Update();

private:
    RenderWindow win; /**< SFML RenderWindow instance used to draw board & pieces to the screen*/
    RectangleShape squares[8][8]; /**< An 8x8 array of SFML RectangleShape objects representing the squares on the chess board*/
    Color sColors[2]; /**< An array of 2 SFML Color objects representing the displayed color of dark & light squares on the board*/
    bool sColor = 1; /**< Boolean value used to alternate between light & dark color theme when setting color of squares on the board*/
    float sW; /**< The with of one square on the board, scaled to fit the game window*/
    float sH; /**< The height of one square on the board, scaled to fit the game window*/
    int selectIndex = -1; /**< The index of the currently selected piece in the array pieces[64] (set to -1 when no piece is selected)*/
    bool pieceSelected = false; /**< Boolean value indicating whether or not a piece is currently selected*/
    int turn = 0; /**< Integer representing which player's turn it is (0 for white to move, 1 for black to move)*/
    cPiece* pieces[64]; /**< An array of references to cPiece objects representing pieces on the board (each piece occupies 1 square initially, & pieces with ID of 0 are never drawn)*/
    std::vector<cMove*> moveHist; /**< A vector of references to cMove objects, which represents the history of all moves completed in the game*/
    int boardMap[8][8] = { {2, 1, 0, 0, 0, 0, -1, -2},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {5, 1, 0, 0, 0, 0, -1, -5},
                            {6, 1, 0, 0, 0, 0, -1, -6},
                            {4, 1, 0, 0, 0, 0, -1, -4},
                            {3, 1, 0, 0, 0, 0, -1, -3},
                            {2, 1, 0, 0, 0, 0, -1, -2} }; /**< An 8x8 array which defines the starting positions of all pieces on the board (value of each element represents a piece ID, and the i-j indices of that element are the x-y coordinates of its square on the board) */

    /**
     * @brief Draws all squares of the chess board to the screen
    */
    void drawSquares();

    /**
     * @brief Sets position of all chess pieces & draws them to the screen
    */
    void drawPieces();

    /**
     * @brief Sets scale of all chess pieces to fit within squares on the board
    */
    void scalePieces();

    /**
     * @brief Checks if a given square on the chess board is occupied by any piece
     * 
     * Returns true if the given square currently contains any piece, otherwise returns false
     * 
     * @param sqI The x-coordinate of the square being checked
     * @param sqJ The y-coordinate of the square being checked
     * 
     * @note prefer isOccupied() over getPiece() when access to cPiece object attributes is not strictly necessary
    */
    bool isOccupied(int sqI, int sqJ);

    /**
     * @brief Returns a pointer to the chess piece at a target square
     *  
     * @param sqI The x-index of the target square
     * @param sqJ The y-index of the target square
     * @return Pointer to a cPiece object if the target square was occupied, otherwise nullptr
     * @note Use only when access to cPiece object attributes is necessary - otherwise use isOccupied()
    */
    cPiece* getPiece(int sqI, int sqJ);
    

    /**
     * @brief Attempts to move a target piece to a target square & if move is completed successfully, adds it to the move history
     * 
     * Checks rules for check & checkmate, and checks rules for special-case moves (i.e. castle moves) to determine if move is legal.\n 
     * If move is deemed illegal, it is not completed. If move is deemed legal, the following actions are taken based on move type:\n 
     * 
     * - Normal moves:\n 
     *    1) Target piece is moved to the given destination square ( cPiece position members are updated )\n 
     *    2) cMove instance for a normal move is created & added to the move history\n 
     * 
     * - Capture moves:\n 
     *    1) Enemy piece at destination square is removed from the board\n 
     *    2) Target piece is moved to the given destination square ( cPiece position members are updated )\n 
     *    3)  cMove instance for a capture move is created & added to the move history\n 
     * 
     * - Castle moves:\n 
     *    1) Target piece (king) is moved to the given destination square ( cPiece position members are updated )\n 
     *    2) Secondary piece (rook) is moved to the appropriate destination square based on destination square of the king ( cPiece position members are updated )\n 
     *    3) cMove instance for a castle move is created & added to the move history\n 
     * 
     * @param piece A pointer to the chess piece that is being moved
     * @param targetI The x-index of the target square
     * @param targetJ The y-index of the target square
     * @return true if the piece was moved successfully (move deemed legal), otherwise false
     * @note This function only checks rules for check, checkmate, and castle moves. Always call legalMove() first to check general movement rules for the given piece
    */
    bool doMove2(cPiece* piece, int targetI, int targetJ);

    // un-does the last move, removing it from the moveHist vector and returning all pieces to their 
    // original position before that move
    // Returns true if move could be un-done, otherwise returns false
    
    
    /**
    * @brief Un-does most recent move
    * 
    * Removes the most recent move from the game's move history, and returns all moved pieces to their original positions before that move was executed.\n 
    * If any pieces were captured in that move, they are returned to the board in their last known position.
    * 
    * @return true if a move was un-done successfully. False if there were no moves to un-do (move history empty)
    */
    bool unDoMove();

    //Prints current move history for the entire game to the console
    
    
    /**
    * @brief Prints the current move history for the game to the console, in order of most recent to least recent
    */
    void printHist();

    // Takes a pointer to a cPiece object and attempts to move it to square at coordinates [targetI][targetJ]
    // If target square contains enemy piece, capture it
    // Return true if move was executed successfully, otherwise return false
    // NOTE: does not fully check legality of move - always check legality using legalMove() first
    bool doMove(cPiece* piece, int targetI, int targetJ);

    /**
    * @brief Checks if the given player is in check
    * 
    * Prints whether or not given player is in check to the console
    * 
    * @param player The player being checked for check (0 for white, 1 for black)
    * @return True if the given player is in check, otherwise false
    */
    bool checkCheck(int player);

    //Returns true if player is in checkmate, otherwise returns false
    bool mateCheck(int player);

    // Checks if moving cPiece object 'piece' to square with coords [newSqI][newSqJ] is legal
    // Returns true if move is legal, otherwise returns false
    // NOTE: does not execute move or modify any pieces - use doMove() to execute move after checking legality
    bool legalMove(cPiece piece, int newSqI, int newSqJ);
};