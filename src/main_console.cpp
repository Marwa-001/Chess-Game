// #include <iostream>
// #include <string>
// #include <cctype>
// #include "../include/chess_board.h"

// using namespace std;

// bool parseInput(const string &input, int &x, int &y)
// {
//     if (input.length() != 2)
//         return false;

//     char file = tolower(input[0]);
//     char rank = input[1];

//     if (file < 'a' || file > 'h')
//         return false;
//     if (rank < '1' || rank > '8')
//         return false;

//     y = file - 'a';
//     x = '8' - rank;
//     return true;
// }

// int main() {
//     ChessBoard board;
//     string input;
//     Color currentTurn = Color::White;

//     cout << "==== Chess Console ====\n";
//     cout << "Enter moves as [from][to] (e.g. 'e2e4')\n";
//     cout << "Castling: '0-0' (kingside) or '0-0-0' (queenside)\n";
//     cout << "Type 'quit' to exit\n\n";

//     while (true) {
//         board.display();
        
//         // Check game state first
//         if (board.isGameOver()) {
//             cout << "Game over!\n";
//             break;
//         }
//         if (board.isStalemate(currentTurn)) {
//             cout << "\nSTALEMATE! Game ends in a draw.\n";
//             break;
//         }
//         if (board.isCheckmate(currentTurn)) {
//             cout << "\nCHECKMATE! " 
//                  << (currentTurn == Color::White ? "Black" : "White") 
//                  << " wins!\n";
//             break;
//         }
//         if (board.isKingInCheck(currentTurn)) {
//             cout << "CHECK! You must protect your king!\n";
//         }

//         cout << (currentTurn == Color::White ? "White" : "Black") << "'s move: ";
//         getline(cin, input);

//         if (input == "quit") break;

//         // Handle castling notation first
//         if (input == "0-0" || input == "0-0-0") {
//             bool kingside = (input == "0-0");
//             if (board.tryCastling(currentTurn, kingside)) {
//                 currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
//             } else {
//                 cout << "Castling not allowed!\n";
//             }
//             continue;
//         }

//         // Handle standard coordinate moves
//         int fromX, fromY, toX, toY;
//         if (input.length() != 4 ||
//             !parseInput(input.substr(0, 2), fromX, fromY) ||
//             !parseInput(input.substr(2, 2), toX, toY)) {
//             cout << "Invalid format! Use like 'a2a4' or '0-0' for castling\n";
//             continue;
//         }

//         // Handle promotion moves
//         Pieces* movingPiece = board.getPiece(fromX, fromY);
//         if (movingPiece && movingPiece->getType() == PieceType::PAWN && 
//             (toX == 0 || toX == 7)) {
//             if (board.movePiece(fromX, fromY, toX, toY)) {
//                 if (board.getPiece(toX, toY)->getType() != PieceType::PAWN) {
//                     currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
//                 }
//             }
//         } 
//         else {
//             // Normal move
//             if (board.movePiece(fromX, fromY, toX, toY)) {
//                 currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
//             } else {
//                 cout << "Invalid move!\n";
//             }
//         }

//         // Clear any leftover input
//         cin.clear();
//     }

//     cout << "Game ended.\n";
//     return 0;
// }