#include "bishop.h"
#include "chess_board.h"

Bishop::Bishop(Color c) : Pieces(c) {}

bool Bishop::isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const {
    // 1. Verify diagonal movement
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    if (dx != dy) return false;  // Not diagonal
    
    // 2. Check path is clear
    int stepX = (toX > fromX) ? 1 : -1;
    int stepY = (toY > fromY) ? 1 : -1;
    
    int x = fromX + stepX;
    int y = fromY + stepY;
    
    while (x != toX && y != toY) {
        if (!board.isEmpty(x, y)) return false;  // Path blocked
        x += stepX;
        y += stepY;
    }
    
    // 3. Validate destination
    return board.isEmpty(toX, toY) ||
           board.getPiece(toX, toY)->getColor() != color;
}

char Bishop::getSymbol() const {
    return (color == Color::White) ? 'B' : 'b';
}