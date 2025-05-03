#pragma once
#include "pieces.h"

class Pawn : public Pieces {
public:
    Pawn(Color c);
    bool isValidMove(int fromX, int fromY, int toX, int toY, const class ChessBoard& board) const override;
    char getSymbol() const override;
    PieceType getType() const override {
        return PieceType::Pawn; // Or however you're defining piece types
    }
};