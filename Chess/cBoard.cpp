#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include "cBoard.h"


using namespace sf;


cBoard::cBoard(float width, float height, const int boardTheme[2][3], std::map<int, std::string> pieceTheme) {
    sW = width / 8;
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
            pieces[index] = new cPiece;
            pieces[index]->ID = boardMap[i][j];
            pieces[index]->x = sW * i;
            pieces[index]->y = sH * j;
            if (pieces[index]->ID > 0)
                pieces[index]->player = 1;
            else if (pieces[index]->ID < 0)
                pieces[index]->player = 0;
            if (pieces[index]->ID != 0) {
                if (!pieces[index]->pTex.loadFromFile(pieceTheme[pieces[index]->ID]))
                    throw "could not load texture";
                pieces[index]->sprite.setTexture(pieces[index]->pTex);
                pieces[index]->draw = true;
            }
            index++;
        }
    }
    scalePieces();
}

cBoard::~cBoard() {
    for (int i = 0; i < 64; i++)
    {
        delete pieces[i];
    }
}

bool cBoard::Update() {
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
            else if (event.key.code == Keyboard::H)
            {
                printHist();
            }
            else if (event.key.code == Keyboard::U)
            {
                if (unDoMove())
                {
                    std::cout << "MOVE UN-DONE\n";
                }
                else
                {
                    std::cout << "COULD NOT UN-DO MOVE\n";
                }
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
                            std::cout << "checking if move was valid for pieces[" << selectIndex << "]\n";
                            if (legalMove(*(pieces[selectIndex]), i, j))
                            {
                                std::cout << "move was valid\n";
                                if (doMove2(pieces[selectIndex], i, j))
                                {
                                    std::cout << "Move completed!\n";
                                    
                                }
                                else
                                {
                                    std::cout << "Could not complete move\n";
                                }
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
                    if ((pieces[k]->sprite.getGlobalBounds().contains(mousePos1)) && pieces[k]->draw)
                    {
                        if (pieces[k]->player == turn)
                        {
                            selectIndex = k;
                            pieceSelected = true;
                            std::cout << "you clicked piece ID: " << pieces[selectIndex]->ID << "\n";
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

void cBoard::drawSquares() {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            win.draw(squares[i][j]);
    }
}

void cBoard::drawPieces() {
    for (int i = 0; i < 64; i++) {
        if (pieces[i]->draw)
        {
            pieces[i]->sprite.setPosition(Vector2f(pieces[i]->x, pieces[i]->y));
            win.draw(pieces[i]->sprite);
        }
    }
}

void cBoard::scalePieces() {
    for (int i = 0; i < 64; i++) {
        float pScaleX = sW / pieces[i]->sprite.getGlobalBounds().width;
        float pScaleY = sH / pieces[i]->sprite.getGlobalBounds().height;
        pieces[i]->sprite.setScale(Vector2f(pScaleX, pScaleY));
        pieces[i]->sprite.setPosition(Vector2f(pieces[i]->x, pieces[i]->y));
    }
}

bool cBoard::isOccupied(int sqI, int sqJ) {
    for (int i = 0; i < 64; i++) //Loop through all pieces, checking if coordinates match sqI & sqJ
    {
        if (!pieces[i]->draw) // Skip check if piece is not drawn
            continue;
        int chkSqI = pieces[i]->x / sW;
        int chkSqJ = pieces[i]->y / sH;
        if ((chkSqI == sqI) && (chkSqJ == sqJ))
        {
            //std::cout << "Invalid move: space was occupied by pieces[" << i << "]\n";
            //std::cout << "pieces[" << i << "] has ID: " << pieces[i]->ID << "\n";
            return true;
        }
    }
    return false;
}

cPiece* cBoard::getPiece(int sqI, int sqJ) {
    for (int i = 0; i < 64; i++) //Loop through all pieces, checking if coordinates match sqI & sqJ
    {
        if (!pieces[i]->draw) // Skip check if piece is not drawn
            continue;
        int chkSqI = pieces[i]->x / sW;
        int chkSqJ = pieces[i]->y / sH;
        if ((chkSqI == sqI) && (chkSqJ == sqJ))
        {
            //std::cout << "Invalid move: space was occupied by pieces[" << i << "]\n";
            //std::cout << "pieces[" << i << "] has ID: " << pieces[i]->ID << "\n";
            return pieces[i];
        }
    }
    return nullptr;
}

bool cBoard::doMove2(cPiece* piece, int targetI, int targetJ)
{
    cPiece* targetPiece = getPiece(targetI, targetJ); // get pointer to piece at target square if it exists
    if ((targetPiece != nullptr)) //If piece was found at target square, it is captured (stop drawing capture piece)
    {
        if (targetPiece->player == turn)// If target square contains players own piece, return false & do not complete move (double check for move validity)
        {
            return false;
        }
        else //target square contains enemy piece, so execute capture & add move to moveHist for both pieces
        {
            targetPiece->draw = false;
            std::cout << "CAPTURE!!!!!!!!!!!\n";
            moveHist.push_back(new cMove(piece, piece->x, piece->y, (sW * targetI), (sH * targetJ), targetPiece));
            piece->x = sW * targetI;
            piece->y = sH * targetJ;
            turn = 1 - turn;
            if (checkCheck(piece->player))//If move puts player in check, undo it
            {
                std::cout << "!!!CANNOT MOVE INTO CHECK - MOVE WILL BE UNDONE!!!\n";
                unDoMove();
                return false;
            }
            else if (checkCheck(1 - (piece->player)))//If move puts enemy in check, look for checkmate
            {
                std::cout << "!!!!CHECK!!!!\n";
                if (mateCheck(1 - (piece->player)))
                {
                    std::cout << "***************GAME OVER***************\n";
                    return true;
                }
            }
            piece->startPos = false;
            return true;
        }
    }
    //Create a new cMove instance and add it to the moveHist vector
    moveHist.push_back(new cMove(piece, piece->x, piece->y, (sW * targetI),(sH * targetJ)));
    piece->x = sW * targetI;
    piece->y = sH * targetJ;
    turn = 1 - turn;
    if (checkCheck(piece->player))//If move puts player in check, undo it
    {
        std::cout << "!!!CANNOT MOVE INTO CHECK - MOVE WILL BE UNDONE!!!\n";
        unDoMove();
        return false;
    }
    else if (checkCheck(1 - (piece->player)))//If move puts enemy in check, print CHECK
    {
        std::cout << "!!!!CHECK!!!!\n";
        if (mateCheck(1 - (piece->player)))
        {
            std::cout << "***************GAME OVER***************\n";
            return true;
        }
    }
    piece->startPos = false;
    return true;
}

bool cBoard::unDoMove() {
    if (moveHist.empty())//If vector is empty (no moves to undo) return false
    {
        return false;
    }

    cMove* last = moveHist.back();
    last->movPiece->x = last->startX; //Revert x and y members of primary moved piece
    last->movPiece->y = last->startY;

    if (last->capPiece != nullptr) //If piece was captured in move, re-draw it
    {
        last->capPiece->draw = true;
    }

    turn = 1 - turn; //Revert turn
    moveHist.pop_back(); // Remove last element from moveHist
    delete last; // Free memory for un-used cMove object
    return true;
}

void cBoard::printHist() {
    std::cout << "***********PRINTING MOVE HISTORY************\n";
    for (auto ir = moveHist.rbegin(); ir != moveHist.rend(); ++ir)
    {
        std::cout << "-----------------------------------------------------\n";
        std::cout << "Moved piece ID: " << (*ir)->movPiece->ID << "\n";
        std::cout << "Start X: " << (*ir)->startX/sW << "\n";
        std::cout << "Start Y: " << (*ir)->startY / sH << "\n";
        std::cout << "End X: " << (*ir)->endX / sW << "\n";
        std::cout << "End Y: " << (*ir)->endY / sH << "\n";
        if ((*ir)->capPiece != nullptr)
        {
            std::cout << "Captured piece ID: " << (*ir)->capPiece->ID << "\n";
        }
        std::cout << "-----------------------------------------------------\n";
    }
    std::cout << "*************END OF MOVE HISTORY************\n";
}

bool cBoard::doMove(cPiece* piece, int targetI, int targetJ) {
    cPiece* targetPiece = getPiece(targetI, targetJ); // get pointer to piece at target square if it exists
    if ((targetPiece != nullptr)) //If piece was found at target square, it is captured (stop drawing capture piece)
    {
        if (targetPiece->player != turn)
        {
            std::cout << "CAPTURE!!!!!!!!!!!\n";
            targetPiece->draw = false;
        }
        else // If target square contains players own piece, return false & do not complete move (double check for move validity)
        {
            return false;
        }
    }
    piece->x = sW * targetI;
    piece->y = sH * targetJ;
    return true;
}

bool cBoard::checkCheck(int player) {
    std::cout << "-------------------Start CheckCheck()----------------------\n";
    std::cout << "Checking check for player: " << player << "\n";
    cPiece* king = nullptr;
    std::vector<cPiece*> enemyPieces;
    for (int i = 0; i < 64; i++) // Loop through all pieces
    {
        if ((std::abs(pieces[i]->ID) == 6) && pieces[i]->player == player) // Save pointer to player's king
        {
            //std::cout << "King found for player: " << player << "\n";
            king = pieces[i];
        }
        else if ((pieces[i]->player != player) && pieces[i]->draw) //Add all drawn enemy pieces to the vector enemyPieces
        {
            enemyPieces.push_back(pieces[i]);
        }
    }

    std::cout << enemyPieces.size() << " Enemy pieces found\n";

    if (king != nullptr) //Ensure king pointer was set to valid piece
    {
        while (!enemyPieces.empty())//For each enemy piece, check if moving to kngs current square is a valid move
        {
            //std::cout << "Checking if piece can move to square: [" << (king->x)/sW << "][" << (king->y)/sH << "]\n";
            if (legalMove(*(enemyPieces.back()), (king->x)/sW, (king->y)/sH))
            {
                return true; // If enemy piece can legally move to kings square, king is in check
                std::cout << "Check found for player: " << player << "\n";
                std::cout << "--------------------End checkCheck()-----------------------\n";
            }
            enemyPieces.pop_back();
        }
    }
    std::cout << "No check found for player: " << player << "\n";
    std::cout << "--------------------End checkCheck()-----------------------\n\n";
    return false; //No enemy piece can move to kings current square, so king is not in check
}

bool cBoard::mateCheck(int player)
{
    std::string plr = "none";
    std::string opponent = "none";
    if (player == 0) // Set strings for player and opponent (used to display game results)
    {
        plr = "White";
        opponent = "Black";
    }
    else if (player == 1)
    {
        plr = "Black";
        opponent = "White";
    }
    std::cout << "###################Start mateCheck()######################\n";
    std::cout << "Checking checkmate for player: " << plr << "\n";
    std::vector<cPiece*> friendlyPieces;
    for (int i = 0; i < 64; i++) // Loop through all pieces
    {
        if ((pieces[i]->player == player) && pieces[i]->draw) //Add all drawn friendly pieces to the vector friendlyPieces
        {
            friendlyPieces.push_back(pieces[i]);
        }
    }

    std::cout << friendlyPieces.size() << " friendly pieces found\n";

    while (!friendlyPieces.empty())//For each friendly piece, check if moving to any square escapes check
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (legalMove(*(friendlyPieces.back()), i, j))
                {
                    if (doMove2(friendlyPieces.back(), i, j))
                    {
                        std::cout << "Legal move found for player: " << plr << "\n";
                        std::cout << plr << " can move piece ID " << friendlyPieces.back()->ID << " to square ["<< i << "][" << j << "]\n";
                        std::cout << "####################End mateCheck()#######################\n";
                        unDoMove();
                        return false; //At least one legal move found which escapes check, so player is not in checkmate
                    }
                }
                
            }
        }
        friendlyPieces.pop_back();
    }
    std::cout << "No valid moves found for player: " << plr << "\n";
    std::cout << "CHECKMATE! PLAYER " << opponent << " WINS!!!!!!!\n";
    std::cout << "####################End mateCheck()#######################\n";
    return true; //No valid moves found to escape check, so player is in checkmate
}

bool cBoard::legalMove(cPiece piece, int newSqI, int newSqJ) {
    int oldSqI = piece.x / sW;
    int oldSqJ = piece.y / sH;
    switch (piece.ID) //Check rules based on piece ID
    {
    case 1:     //Rules for black pawn
        //std::cout << "checking rules for black pawn...\n";

        //Rule #1: Pawn on starting square can move 2 spaces forward only when neither of those spaces is occupied
        if ((oldSqJ == 1) && (newSqI == oldSqI) && (newSqJ == oldSqJ + 2))
        {
            if (isOccupied(newSqI, oldSqJ + 1) || isOccupied(newSqI, newSqJ))
                return false;
            return true;
        }
                
        //Rule #2: Pawn can always move 1 space forward, except when the destination is occupied
        if ((newSqI == oldSqI) && (newSqJ == oldSqJ + 1))
        {
            if (isOccupied(newSqI, newSqJ))
            {
                return false;
            }
            return true;
        }

        //Rule #3: Pawn can move 1 square diagonally if the destinaton is occupied by an enemy piece, thus capturing that piece
        if ((newSqJ == oldSqJ + 1) && ((newSqI == oldSqI - 1) || (newSqI == oldSqI + 1)))
        {
            cPiece* foundPiece = getPiece(newSqI, newSqJ);
            if ((foundPiece != nullptr))
            {
                if (foundPiece->player != piece.player)
                {
                    return true;
                }
            }
            return false;
        }

        else
            //std::cout << "Invalid move: no rules implemented for desired move\n";
            return false;
        break;
    case -1:     //Rules for white pawn
        //std::cout << "checking rules for white pawn...\n";

        //Rule #1: Pawn on starting square can move 2 spaces forward only when neither of those spaces is occupied
        if ((oldSqJ == 6) && (newSqI == oldSqI) && (newSqJ == oldSqJ - 2))
        {
            if (isOccupied(newSqI, oldSqJ - 1) || isOccupied(newSqI, newSqJ))
                return false;
            return true;
        }

        //Rule #2: Pawn can always move 1 space forward, except when the destination is occupied
        if ((newSqI == oldSqI) && (newSqJ == oldSqJ - 1))
        {
            if (isOccupied(newSqI, newSqJ))
            {
                return false;
            }
            return true;
        }

        //Rule #3: Pawn can move 1 square diagonally if the destinaton is occupied by an enemy piece, thus capturing that piece
        if ((newSqJ == oldSqJ - 1) && ((newSqI == oldSqI - 1) || (newSqI == oldSqI + 1)))
        {
            cPiece* foundPiece = getPiece(newSqI, newSqJ);
            if ((foundPiece != nullptr))
            {
                if (foundPiece->player != piece.player)
                {
                    return true;
                }
            }
            return false;
        }
        else
            //std::cout << "Invalid move: no rules implemented for desired move\n";
        return false;
        break;
    case 2:     //Rules for black rook
        //std::cout << "checking rules for black rook...\n";

        //Rule #1: Rook can move any # of squares forward, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqI == oldSqI) && (newSqJ > oldSqJ))
        {
            for (int j = oldSqJ + 1; j < newSqJ;j++) //Check all squares along path to destination
            {
                if (isOccupied(newSqI, j)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        //Rule #2: Rook can move any # of squares backward, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqI == oldSqI) && (newSqJ < oldSqJ))
        {
            for (int j = oldSqJ - 1; j > newSqJ;j--) //Check all squares along path to destination
            {
                if (isOccupied(newSqI, j)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        //Rule #3: Rook can move any # of squares left, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqJ == oldSqJ) && (newSqI < oldSqI))
        {
            for (int i = oldSqI - 1; i > newSqI;i--) //Check all squares along path to destination
            {
                if (isOccupied(i, newSqJ)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        //Rule #4: Rook can move any # of squares right, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqJ == oldSqJ) && (newSqI > oldSqI))
        {
            for (int i = oldSqI + 1; i < newSqI;i++) //Check all squares along path to destination
            {
                if (isOccupied(i, newSqJ)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        return false;
        break;
    case -2:     //Rules for white rook
        //std::cout << "checking rules for white rook...\n";

        //Rule #1: Rook can move any # of squares up, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqI == oldSqI) && (newSqJ < oldSqJ))
        {
            for (int j = oldSqJ - 1; j > newSqJ;j--) //Check all squares along path to destination
            {
                if (isOccupied(newSqI, j)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        //Rule #2: Rook can move any # of squares down, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqI == oldSqI) && (newSqJ > oldSqJ))
        {
            for (int j = oldSqJ + 1; j < newSqJ;j++) //Check all squares along path to destination
            {
                if (isOccupied(newSqI, j)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        //Rule #3: Rook can move any # of squares left, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqJ == oldSqJ) && (newSqI < oldSqI))
        {
            for (int i = oldSqI - 1; i > newSqI;i--) //Check all squares along path to destination
            {
                if (isOccupied(i, newSqJ)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }

        //Rule #4: Rook can move any # of squares right, as long as no square along its path is occupied.
        //         If the above check passes & the destination square contains an enemy piece, it is captured
        if ((newSqJ == oldSqJ) && (newSqI > oldSqI))
        {
            for (int i = oldSqI + 1; i < newSqI;i++) //Check all squares along path to destination
            {
                if (isOccupied(i, newSqJ)) //If any squares along path are occupied, move is invalid
                {
                    return false;
                }
            }
            cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
            if (foundPiece != nullptr)
            {
                if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                {
                    return false;
                }
            }
            return true;
        }
        return false;
        break;
    case 3:     //Rules for black knight are identical to rules for white knight (fall through when piece is black knight)
    case -3:    //Rules for white knight
    {
        //std::cout << "checking rules for knight...\n";

        //Rule #1: For knight at square [i][j], there are up to 8 possible moves, defined as [i+X[index]][j+Y[index]]
        //         where the vectors X & Y are: X[8] = { 2, 2, -2, -2,  1, 1, -1, -1} 
        //                                      Y[8] = {-1, 1, -1,  1, -2, 2, -2,  2}
        // If the destination square is occupied by players own piece, move is invalid
        int X[8] = { 2, 2, -2, -2,  1, 1, -1, -1 };
        int Y[8] = { -1, 1, -1,  1, -2, 2, -2,  2 };
        for (int index = 0; index < 8; index++)
        {
            if ((newSqI == (oldSqI + X[index])) && (newSqJ == (oldSqJ + Y[index])))
            {
                cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
                if (foundPiece != nullptr)
                {
                    if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
        break;
    }
    case 4:     //Rules for black bishop are identical to rules for white bishop (fall through when piece is black bishop)
    case -4:    //Rules for white bishop
    {
        //std::cout << "checking rules for bishop...\n";

        //Rule #1: Bishop can move any # of squares in any diagonal direction (up/left, up/right, down/left, down/right)
        if (std::abs(oldSqI - newSqI) != std::abs(oldSqJ - newSqJ))
        {
            return false;
        }

        //Rule #2: If destination square contains players own piece, move is invalid
        cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
        if (foundPiece != nullptr)
        {
            if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
            {
                return false;
            }
        }

        //Rule #3: Bishop move is invalid if any square along its path is occupied
        if ((newSqI < oldSqI) && (newSqJ < oldSqJ)) //Check path for up/left moves
        {
            //std::cout << "UP/LEFT MOVE\n";
            int j = oldSqJ - 1;
            for (int i = oldSqI - 1; i > newSqI; i--) // Check all squares along path
            {
                //std::cout << "BISHOP - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                {
                    //std::cout << "OCCUPIED!!!!!!!!!\n";
                    return false;
                }
                j--;
            }
            return true;
        }

        if ((newSqI > oldSqI) && (newSqJ < oldSqJ)) //Check path for up/right moves
        {
            //std::cout << "UP/RIGHT MOVE\n";
            int j = oldSqJ - 1;
            for (int i = oldSqI + 1; i < newSqI; i++)
            {
                //std::cout << "BISHOP - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                {
                    //std::cout << "OCCUPIED!!!!!!!!!\n";
                    return false;
                }
                j--;
            }
            return true;
        }

        if ((newSqI < oldSqI) && (newSqJ > oldSqJ)) //Check path for down/left moves
        {
            //std::cout << "DOWN/LEFT MOVE\n";
            int j = oldSqJ + 1;
            for (int i = oldSqI - 1; i > newSqI; i--)
            {
                //std::cout << "BISHOP - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                {
                    //std::cout << "OCCUPIED!!!!!!!!!\n";
                    return false;
                }
                j++;
            }
            return true;
        }

        if ((newSqI > oldSqI) && (newSqJ > oldSqJ)) //Check path for down/right moves
        {
            //std::cout << "DOWN/RIGHT MOVE\n";
            int j = oldSqJ + 1;
            for (int i = oldSqI + 1; i < newSqI; i++)
            {
                //std::cout << "BISHOP - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                {
                    //std::cout << "OCCUPIED!!!!!!!!!\n";
                    return false;
                }
                j++;
            }
            return true;
        }
        return false;
        break;
    }
    case 5:     //Rules for black queen are identical to rules for white queen (fall through when piece is black queen)
    case -5:    //Rules for white queen
    {
        //std::cout << "checking rules for queen...\n";

        //Rule #1: Queen can move any # of squares in any diagonal direction (up/left, up/right, down/left, down/right),
        //          OR in any cardinal direction (up, down, left, right)
        bool diagonal = false;
        bool horizontal = false;
        bool vertical = false;
        if (std::abs(oldSqI - newSqI) == std::abs(oldSqJ - newSqJ)) //Check if move is diagonal
        {
            diagonal = true;
        }
        else if (oldSqJ == newSqJ) //Check if move is horizontal
        {
            horizontal = true;
        }
        else if (oldSqI == newSqI) //check if move is vertical
        {
            vertical = true;
        }
        else                       //If move is not diagonal, vertical, or horizontal, it is not valid
        {
            return false;
        }

        //Rule #2: If destination square contains players own piece, move is invalid
        cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
        if (foundPiece != nullptr)
        {
            if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
            {
                return false;
            }
        }

        //Rule #3: Queen move is invalid if any square along its path is occupied
        if (diagonal) //Check path for diagonal moves
        {
            if ((newSqI < oldSqI) && (newSqJ < oldSqJ)) //Check path for up/left moves
            {
                //std::cout << "UP/LEFT MOVE\n";
                int j = oldSqJ - 1;
                for (int i = oldSqI - 1; i > newSqI; i--) // Check all squares along path
                {
                    //std::cout << "QUEEN - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                    if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                    {
                        //std::cout << "OCCUPIED!!!!!!!!!\n";
                        return false;
                    }
                    j--;
                }
                return true;
            }

            if ((newSqI > oldSqI) && (newSqJ < oldSqJ)) //Check path for up/right moves
            {
                //std::cout << "UP/RIGHT MOVE\n";
                int j = oldSqJ - 1;
                for (int i = oldSqI + 1; i < newSqI; i++)
                {
                    //std::cout << "QUEEN - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                    if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                    {
                        //std::cout << "OCCUPIED!!!!!!!!!\n";
                        return false;
                    }
                    j--;
                }
                return true;
            }

            if ((newSqI < oldSqI) && (newSqJ > oldSqJ)) //Check path for down/left moves
            {
                //std::cout << "DOWN/LEFT MOVE\n";
                int j = oldSqJ + 1;
                for (int i = oldSqI - 1; i > newSqI; i--)
                {
                    //std::cout << "QUEEN - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                    if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                    {
                        //std::cout << "OCCUPIED!!!!!!!!!\n";
                        return false;
                    }
                    j++;
                }
                return true;
            }

            if ((newSqI > oldSqI) && (newSqJ > oldSqJ)) //Check path for down/right moves
            {
                //std::cout << "DOWN/RIGHT MOVE\n";
                int j = oldSqJ + 1;
                for (int i = oldSqI + 1; i < newSqI; i++)
                {
                    //std::cout << "QUEEN - CHECKING SQUARE ALONG PATH: [" << i << "][" << j << "]\n";
                    if (isOccupied(i, j)) //If any squares along path are occupied, move is invalid
                    {
                        //std::cout << "OCCUPIED!!!!!!!!!\n";
                        return false;
                    }
                    j++;
                }
                return true;
            }
        }

        if (vertical) //Check path for vertical moves
        {
            //std::cout << "VERTICAL MOVE!!!!!\n";
            if (newSqJ < oldSqJ) //Check path for vertical-up moves
            {
                //std::cout << "GOING UP\n";
                for (int j = oldSqJ - 1; j > newSqJ;j--) //Check all squares along path to destination
                {
                    //std::cout << "QUEEN - CHECKING SQUARE ALONG PATH: [" << newSqI << "][" << j << "]\n";
                    if (isOccupied(newSqI, j)) //If any squares along path are occupied, move is invalid
                    {
                        return false;
                    }
                }
                return true;
            }
            if (newSqJ > oldSqJ) //Check path for vertical-down moves
            {
                //std::cout << "GOING DOWN\n";
                for (int j = oldSqJ + 1; j < newSqJ;j++) //Check all squares along path to destination
                {
                    if (isOccupied(newSqI, j)) //If any squares along path are occupied, move is invalid
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        if (horizontal) //Check path for horizontal moves
        {
            if (newSqI < oldSqI) //Check path for horizontal-left moves
            {
                for (int i = oldSqI - 1; i > newSqI;i--) //Check all squares along path to destination
                {
                    if (isOccupied(i, newSqJ)) //If any squares along path are occupied, move is invalid
                    {
                        return false;
                    }
                }
                return true;
            }
            if (newSqI > oldSqI) //Check path for horizontal-right moves
            {
                for (int i = oldSqI + 1; i < newSqI;i++) //Check all squares along path to destination
                {
                    if (isOccupied(i, newSqJ)) //If any squares along path are occupied, move is invalid
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
        break;
    }
    case 6:     //Rules for black king
    {
        //std::cout << "Checking rules for black king\n";

        //Rule #1: If destination square contains players own piece, move is invalid
        cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
        if (foundPiece != nullptr)
        {
            if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
            {
                return false;
            }
        }

        //Rule #2: King can move 1 square in any direction
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++) //Check if destination is oldSqI +/- 1 & oldSqJ +/- 1
            {
                if ((i == 0) && (j == 0))
                {
                    continue;
                }
                if ((newSqI == oldSqI+i) && (newSqJ == oldSqJ+j))
                {
                    return true;
                }
            }
        }
        return false;
        break;
    }
    case -6:     //Rules for white king
    {
        //std::cout << "Checking rules for white king\n";

        //Rule #1: If destination square contains players own piece, move is invalid
        cPiece* foundPiece = getPiece(newSqI, newSqJ); //Check for piece at destination
        if (foundPiece != nullptr)
        {
            if (foundPiece->player == piece.player) //If piece at destination does not belong to enemy, move is invalid
            {
                return false;
            }
        }

        //Rule #2: King can move 1 square in any direction
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++) //Check if destination is oldSqI +/- 1 & oldSqJ +/- 1
            {
                if ((i == 0) && (j == 0))
                {
                    continue;
                }
                if ((newSqI == oldSqI + i) && (newSqJ == oldSqJ + j))
                {
                    return true;
                }
            }
        }
        return false;
        break;
    }
    default:
        return false;
        break;
    }
}

