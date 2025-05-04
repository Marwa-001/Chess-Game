#pragma once

enum class Color { White, Black };
enum class PieceType {
    KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN
};

class Pieces {
protected:
    Color color;
public:
    Pieces(Color c) : color(c) {}
    virtual ~Pieces() = default;
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const class ChessBoard& board) const = 0;
    virtual char getSymbol() const = 0;
    Color getColor() const { return color; }
    virtual PieceType getType() const = 0;
};