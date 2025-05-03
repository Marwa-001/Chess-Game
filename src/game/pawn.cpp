#include "../../include/pawn.h"
#include "../../include/chess_board.h"

Pawn::Pawn(Color c) : Pieces(c) {}

bool Pawn::isValidMove(int fromX, int fromY, int toX, int toY, const ChessBoard& board) const {
    int direction = (color == Color::White) ? -1 : 1;
    
    // Normal move forward
    if (fromY == toY) {
        // Single step
        if (toX == fromX + direction && board.isEmpty(toX, toY)) {
            return true;
        }
        // Double step from starting position
        if (toX == fromX + 2 * direction && board.isEmpty(toX, toY) && 
            board.isEmpty(fromX + direction, toY)) {
            bool isStartingPos = (color == Color::White) ? (fromX == 6) : (fromX == 1);
            return isStartingPos;
        }
    }
    // Capture move
    else if (abs(toY - fromY) == 1 && toX == fromX + direction) {
        if (!board.isEmpty(toX, toY) && board.getPiece(toX, toY)->getColor() != color) {
            return true;
        }
    }
    return false;
}

char Pawn::getSymbol() const {
    return (color == Color::White) ? 'P' : 'p';
}