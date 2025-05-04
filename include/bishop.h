#pragma once
#include "pieces.h"

class Bishop : public Pieces {
public:
    Bishop(Color c);
    bool isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const override;
    char getSymbol() const override;
    PieceType getType() const override { return PieceType::BISHOP; }
};