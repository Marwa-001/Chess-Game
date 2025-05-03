#include "../../include/queen.h"
#include "../../include/chess_board.h"

Queen::Queen(Color c) : Pieces(c) {}

bool Queen::isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const {
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);

    // 1. Must move straight or diagonally (combines rook + bishop)
    if (dx != 0 && dy != 0 && dx != dy) return false;

    // 2. Check path is clear
    int stepX = (toX > fromX) ? 1 : (toX < fromX) ? -1 : 0;
    int stepY = (toY > fromY) ? 1 : (toY < fromY) ? -1 : 0;

    int x = fromX + stepX;
    int y = fromY + stepY;

    while (x != toX || y != toY) {
        if (!board.isEmpty(x, y)) return false;
        x += stepX;
        y += stepY;
    }

    // 3. Validate destination
    return board.isEmpty(toX, toY) || 
           board.getPiece(toX, toY)->getColor() != color;
}

char Queen::getSymbol() const {
    return (color == Color::White) ? 'Q' : 'q';
}