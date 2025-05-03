#include "../../include/knight.h"
#include "../../include/chess_board.h"

Knight::Knight(Color c) : Pieces(c) {}

bool Knight::isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const {
    // 1. Check L-shaped movement (8 possible moves)
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) {
        return false;
    }

    // 2. Check destination (empty or enemy)
    if (board.isEmpty(toX, toY)) {
        return true;
    }
    return board.getPiece(toX, toY)->getColor() != color;
}

char Knight::getSymbol() const {
    return (color == Color::White) ? 'N' : 'n';  // 'N' for knight
}
