#pragma once
#include "pieces.h"

class Rook : public Pieces {
public:
    Rook(Color c);
    bool isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const override;
    char getSymbol() const override;
    
    // Special for castling
    bool hasMoved = false; // Track if rook moved (for castling)
    PieceType getType() const override { return PieceType::ROOK; }
};