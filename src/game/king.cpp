#include "../../include/king.h"
#include "../../include/chess_board.h"

King::King(Color c) : Pieces(c) {}

bool King::isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const {
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);

    // 1. Standard king movement (1 square any direction)
    if (dx <= 1 && dy <= 1) {
        // Destination must be empty or enemy
        return board.isEmpty(toX, toY) || 
               board.getPiece(toX, toY)->getColor() != color;
    }

    // 2. Castling (handled separately in ChessBoard)
    return false; // Castling validation happens in ChessBoard::movePiece()
}

char King::getSymbol() const {
    return (color == Color::White) ? 'K' : 'k';
}
