#pragma once
#include <array>
#include <memory>
#include "pieces.h"
#include "rook.h"
#include<vector>

struct Position{
    int row, col;
};
class ChessBoard {
private:
    std::array<std::array<std::unique_ptr<Pieces>, 8>, 8> board;
    bool gameOver = false;
    bool checkmate=false;
    Color currentTurn;
    bool isCastlingPathClear(int row, int fromCol, int toCol) const;
    bool isCastlingValid(int kingX, int kingY, int rookX, int rookY, Color color);
    bool isPathClear(int fromX, int fromY, int toX, int toY) const;
    void promotePawn(int x, int y);
public:
    ChessBoard();
    bool isGameOver() const { return gameOver; }
    Color getCurrentTurn() const { return currentTurn; }
    void initializeBoard();
    bool movePiece(int fromX, int fromY, int toX, int toY);
    bool isEmpty(int x, int y) const;
    Pieces* getPiece(int x, int y) const;
    void display() const;
    Position findKingPos(Color kingColor) const;
    bool canEnemyAttack(int targetX, int targetY, Color enemyColor) const;
    bool isKingInCheck(Color kingColor) const;
    bool doesMoveExposeKing(int fromX, int fromY, int toX, int toY, Color movingColor);
    bool isMoveValid(int fromX, int fromY, int toX, int toY, Color currentTurn);
    std::vector<Position> getAttackPath(Position attacker, Position king);
    bool hasAnyLegalMove(Color color);
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    bool tryCastling(Color color, bool kingside);

    int enPassantTargetRow = -1;
int enPassantTargetCol = -1;
};