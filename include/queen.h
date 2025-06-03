#pragma once
#include "pieces.h"

class Queen : public Pieces {
public:
    Queen(Color c);
    bool isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const override;
    char getSymbol() const override;
    PieceType getType() const override {
        return PieceType::QUEEN;
    }
};