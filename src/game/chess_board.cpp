#include "../../include/chess_board.h"
#include "../../include/pawn.h"
#include "../../include/knight.h"
#include "../../include/bishop.h"
#include "../../include/king.h"
#include "../../include/queen.h"
#include <iostream>
#include <vector>

ChessBoard::ChessBoard() : currentTurn(Color::White)
{
    initializeBoard();
}

std::vector<Position> ChessBoard::getAttackPath(Position attacker, Position king)
{
    std::vector<Position> path;

    // Straight line (rook/queen)
    if (attacker.row == king.row || attacker.col == king.col)
    {
        int stepX = (king.row > attacker.row) ? 1 : (king.row < attacker.row) ? -1
                                                                              : 0;
        int stepY = (king.col > attacker.col) ? 1 : (king.col < attacker.col) ? -1
                                                                              : 0;

        int x = attacker.row + stepX;
        int y = attacker.col + stepY;

        while (x != king.row || y != king.col)
        {
            path.push_back({x, y});
            x += stepX;
            y += stepY;
        }
    }
    // Diagonal (bishop/queen)
    else if (abs(attacker.row - king.row) == abs(attacker.col - king.col))
    {
        int stepX = (king.row > attacker.row) ? 1 : -1;
        int stepY = (king.col > attacker.col) ? 1 : -1;

        int x = attacker.row + stepX;
        int y = attacker.col + stepY;

        while (x != king.row && y != king.col)
        {
            path.push_back({x, y});
            x += stepX;
            y += stepY;
        }
    }

    return path;
}

void ChessBoard::initializeBoard()
{
    // Clear the board first
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            board[x][y] = nullptr;
        }
    }

    // Initialize pawns
    for (int y = 0; y < 8; y++)
    {
        board[1][y] = std::make_unique<Pawn>(Color::Black);
        board[6][y] = std::make_unique<Pawn>(Color::White);
    }

    // Place BLACK rooks (top row - a8 and h8)
    board[0][0] = std::make_unique<Rook>(Color::Black); // a8
    board[0][7] = std::make_unique<Rook>(Color::Black); // h8

    // Place WHITE rooks (bottom row - a1 and h1)
    board[7][0] = std::make_unique<Rook>(Color::White); // a1
    board[7][7] = std::make_unique<Rook>(Color::White); // h1

    // Black knights (back row)
    board[0][1] = std::make_unique<Knight>(Color::Black); // b8
    board[0][6] = std::make_unique<Knight>(Color::Black); // g8

    // White knights (front row)
    board[7][1] = std::make_unique<Knight>(Color::White); // b1
    board[7][6] = std::make_unique<Knight>(Color::White); // g1

    // Black bishops (back row)
    board[0][2] = std::make_unique<Bishop>(Color::Black); // c8
    board[0][5] = std::make_unique<Bishop>(Color::Black); // f8

    // White bishops (front row)
    board[7][2] = std::make_unique<Bishop>(Color::White); // c1
    board[7][5] = std::make_unique<Bishop>(Color::White); // f1

    // Kings
    board[0][4] = std::make_unique<King>(Color::Black); // e8
    board[7][4] = std::make_unique<King>(Color::White); // e1

    // Queens
    board[0][3] = std::make_unique<Queen>(Color::Black); // d8
    board[7][3] = std::make_unique<Queen>(Color::White); // d1
                                                         // TODO: Initialize other pieces
}

bool ChessBoard::movePiece(int fromX, int fromY, int toX, int toY)
{
    if (!isMoveValid(fromX, fromY, toX, toY, currentTurn))
    {
        if (isKingInCheck(currentTurn) && !hasAnyLegalMove(currentTurn))
        {
            checkmate = true;
        }
        return false;
    }

    // Capture announcement
    if (board[toX][toY])
    {
        std::cout << board[fromX][fromY]->getSymbol()
                  << " captures "
                  << board[toX][toY]->getSymbol()
                  << " at " << char('a' + toY) << 8 - toX << "!\n";
    }

    // Execute the move - IMPORTANT FIX HERE
    auto movingPiece = std::move(board[fromX][fromY]); // Store the moving piece first
    board[toX][toY] = std::move(movingPiece);          // Then place it at destination

    // Check game state
    Color opponent = (currentTurn == Color::White) ? Color::Black : Color::White;
    if (isCheckmate(opponent))
    {
        std::cout << "CHECKMATE! "
                  << (currentTurn == Color::White ? "White" : "Black")
                  << " wins!\n";
        gameOver = true;
        checkmate = true;
    }
    else if (isKingInCheck(opponent))
    {
        std::cout << "CHECK!\n";
    }
    currentTurn = opponent;
    
    return true;
}

bool ChessBoard::isEmpty(int x, int y) const
{
    return board[x][y] == nullptr;
}

Pieces *ChessBoard::getPiece(int x, int y) const
{
    return board[x][y].get();
}

// Finds the king's position (0-7 coordinates)
Position ChessBoard::findKingPos(Color kingColor) const
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (board[row][col] &&
                board[row][col]->getType() == PieceType::KING &&
                board[row][col]->getColor() == kingColor)
            {
                return Position{row, col};
            }
        }
    }
    throw std::runtime_error("King not found!");
}

// Checks if any enemy piece can attack (x,y)
bool ChessBoard::canEnemyAttack(int targetX, int targetY, Color enemyColor) const
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (board[row][col] &&
                board[row][col]->getColor() == enemyColor &&
                board[row][col]->isValidMove(row, col, targetX, targetY, *this))
            {
                return true;
            }
        }
    }
    return false;
}

// Checks if king is currently in check
bool ChessBoard::isKingInCheck(Color kingColor) const
{
    Position kingPos = findKingPos(kingColor);
    return canEnemyAttack(kingPos.row, kingPos.col,
                          (kingColor == Color::White) ? Color::Black : Color::White);
}

// Simulates move and checks for king exposure
bool ChessBoard::doesMoveExposeKing(int fromX, int fromY, int toX, int toY, Color movingColor)
{
    // Simulate move
    board[toX][toY] = std::move(board[fromX][fromY]);

    // Check king safety
    bool isSafe = !isKingInCheck(movingColor);

    // Undo simulation
    board[fromX][fromY] = std::move(board[toX][toY]);

    return !isSafe;
}

// Complete move validation
bool ChessBoard::isMoveValid(int fromX, int fromY, int toX, int toY, Color currentTurn)
{
    // 1. Basic validation (piece exists and belongs to player)
    if (!board[fromX][fromY] || board[fromX][fromY]->getColor() != currentTurn)
    {
        return false;
    }

    // 2. Check piece movement rules
    if (!board[fromX][fromY]->isValidMove(fromX, fromY, toX, toY, *this))
    {
        return false;
    }

    // 3. Simulate move and check king safety
    if (doesMoveExposeKing(fromX, fromY, toX, toY, currentTurn))
    {
        return false;
    }

    // 4. Special case: If in check, verify the move actually resolves it
    if (isKingInCheck(currentTurn))
    {
        // Simulate the move
        auto temp = std::move(board[toX][toY]);
        board[toX][toY] = std::move(board[fromX][fromY]);
        board[fromX][fromY] = nullptr;

        // Check if king is still in check after this move
        bool stillInCheck = isKingInCheck(currentTurn);

        // Undo simulation
        board[fromX][fromY] = std::move(board[toX][toY]);
        board[toX][toY] = std::move(temp);

        if (stillInCheck)
        {
            return false; // Move doesn't resolve check
        }
    }

    return true;
}

// Checkmate detection
bool ChessBoard::isCheckmate(Color color)
{
    if (!isKingInCheck(color))
        return false;

    Position kingPos = findKingPos(color);
    Color enemyColor = (color == Color::White) ? Color::Black : Color::White;

    // 1. Check all king moves first
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
                continue;

            int x = kingPos.row + dx;
            int y = kingPos.col + dy;

            if (x >= 0 && x < 8 && y >= 0 && y < 8)
            {
                // Skip if square is occupied by friendly piece
                if (board[x][y] && board[x][y]->getColor() == color)
                {
                    continue;
                }

                // Simulate move
                auto captured = std::move(board[x][y]);
                board[x][y] = std::move(board[kingPos.row][kingPos.col]);
                bool inCheck = isKingInCheck(color);

                // Undo simulation
                board[kingPos.row][kingPos.col] = std::move(board[x][y]);
                board[x][y] = std::move(captured);

                if (!inCheck)
                {
                    return false; // King has escape
                }
            }
        }
    }

    // 2. Find all attackers
    std::vector<Position> attackers;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y] && board[x][y]->getColor() == enemyColor)
            {
                if (board[x][y]->isValidMove(x, y, kingPos.row, kingPos.col, *this))
                {
                    attackers.push_back({x, y});
                }
            }
        }
    }

    // 3. If multiple attackers, only king moves could help (already checked)
    if (attackers.size() > 1)
    {
        return true;
    }

    // 4. For single attacker, check blocking/capturing
    Position attacker = attackers[0];
    std::vector<Position> path = getAttackPath(attacker, kingPos);

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y] && board[x][y]->getColor() == color)
            {
                // Can this piece capture the attacker?
                if (isMoveValid(x, y, attacker.row, attacker.col, color))
                {
                    return false;
                }

                // Can this piece block the path?
                for (auto &blockPos : path)
                {
                    if (isMoveValid(x, y, blockPos.row, blockPos.col, color))
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true; // No escapes found
}
bool ChessBoard::hasAnyLegalMove(Color color)
{
    // First check if king can move
    Position kingPos = findKingPos(color);
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
                continue;

            int x = kingPos.row + dx;
            int y = kingPos.col + dy;

            if (x >= 0 && x < 8 && y >= 0 && y < 8)
            {
                // Check if king can move here
                if (isMoveValid(kingPos.row, kingPos.col, x, y, color))
                {
                    return true;
                }
            }
        }
    }

    // Then check all other pieces
    for (int fromX = 0; fromX < 8; fromX++)
    {
        for (int fromY = 0; fromY < 8; fromY++)
        {
            if (board[fromX][fromY] && board[fromX][fromY]->getColor() == color)
            {
                // Skip the king (already checked)
                if (board[fromX][fromY]->getType() == PieceType::KING)
                {
                    continue;
                }

                // Check all possible moves for this piece
                for (int toX = 0; toX < 8; toX++)
                {
                    for (int toY = 0; toY < 8; toY++)
                    {
                        if (isMoveValid(fromX, fromY, toX, toY, color))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool ChessBoard::isStalemate(Color color){
    // 1. King must NOT be in check
    if (isKingInCheck(color)) {
        return false;
    }

    // 2. No legal moves for any piece
    if (hasAnyLegalMove(color)) {
        return false;
    }

    return true;
}

void ChessBoard::display() const
{
    std::cout << "  a b c d e f g h\n";
    for (int x = 0; x < 8; x++)
    {
        std::cout << 8 - x << " ";
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y])
            {
                std::cout << board[x][y]->getSymbol() << " ";
            }
            else
            {
                std::cout << ". ";
            }
        }
        std::cout << 8 - x << "\n";
    }
    std::cout << "  a b c d e f g h\n";

    std::cout << "\n"
              << (currentTurn == Color::White ? "White" : "Black")
              << "'s turn to move\n";
}
