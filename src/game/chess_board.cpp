#include "../../include/chess_board.h"
#include "../../include/pawn.h"
#include "../../include/knight.h"
#include "../../include/bishop.h"
#include "../../include/king.h"
#include "../../include/queen.h"
#include <iostream>

ChessBoard::ChessBoard() : currentTurn(Color::White)
{
    initializeBoard();
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
    // 1. Check if piece exists and is correct color
    if (!board[fromX][fromY] ||
        board[fromX][fromY]->getColor() != currentTurn)
    {
        return false;
    }

    // 2. Validate move (delegates to piece-specific logic)
    if (!board[fromX][fromY]->isValidMove(fromX, fromY, toX, toY, *this))
    {
        return false;
    }

    // 3. Execute move
    board[toX][toY] = std::move(board[fromX][fromY]);

    // 4. Switch turn
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
    return true;

    auto &piece = board[fromX][fromY]; // Get the piece before checking type
    if (piece->getType() == PieceType::Rook)
    {
        dynamic_cast<Rook *>(piece.get())->hasMoved = true;
    }
}

bool ChessBoard::isEmpty(int x, int y) const
{
    return board[x][y] == nullptr;
}

Pieces *ChessBoard::getPiece(int x, int y) const
{
    return board[x][y].get();
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
