#pragma once
#include "pieces.h"

class King : public Pieces {
public:
    King(Color c);
    bool isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const override;
    char getSymbol() const override;
    bool hasMoved = false; // For castling
    PieceType getType() const override {
        return PieceType::King; // Or however you're defining piece types
    }
};