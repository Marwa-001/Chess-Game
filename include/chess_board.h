#pragma once
#include <array>
#include <memory>
#include "pieces.h"
#include "rook.h"

class ChessBoard {
private:
    std::array<std::array<std::unique_ptr<Pieces>, 8>, 8> board;
    
    Color currentTurn;
public:
    ChessBoard();
    // ChessBoard() : currentTurn(Color::White) {}  // White moves first
    void initializeBoard();
    bool movePiece(int fromX, int fromY, int toX, int toY);
    bool isEmpty(int x, int y) const;
    Pieces* getPiece(int x, int y) const;
    void display() const;
};