#include <iostream>
#include <string>
#include <cctype>
#include "../include/chess_board.h"

using namespace std;

bool parseInput(const string &input, int &x, int &y)
{
    if (input.length() != 2)
        return false;

    char file = tolower(input[0]);
    char rank = input[1];

    if (file < 'a' || file > 'h')
        return false;
    if (rank < '1' || rank > '8')
        return false;

    y = file - 'a';
    x = '8' - rank;
    return true;
}

int main()
{
    ChessBoard board;
    string input;
    Color currentTurn = Color::White;

    cout << "==== Chess Console ====\n";
    cout << "Enter moves as [from][to] (e.g. 'e2e4')\n";
    cout << "Type 'quit' to exit\n\n";

    // Force immediate output
    cout << "Initial board:" << endl;
    board.display();
    cout << flush; // Explicitly flush output
    
    // Check for game over before prompting for move
    if (board.isGameOver()) {
        cout << "CHECKMATE! " 
             << (currentTurn == Color::White ? "Black" : "White")
             << " wins!\n";
    }

    while (true)
    {
        board.display();
    
        // Check for checkmate first
        if (board.isCheckmate(currentTurn)) {
            cout << "\nCHECKMATE! " 
                 << (currentTurn == Color::White ? "Black" : "White") 
                 << " wins!\n";
            break;  // Exit the game loop immediately
        }
        
        // Display check warning
        if (board.isKingInCheck(currentTurn)) {
            cout << "CHECK! You must protect your king!\n";
        }
    
        cout << (currentTurn == Color::White ? "White" : "Black") << "'s move: ";

        // Robust input handling
        if (!getline(cin, input))
        {
            cerr << "Input error!\n";
            break;
        }

        if (input == "quit")
            break;

        if (input.empty())
        {
            cout << "Empty input! Try 'e2e4' or 'quit'\n";
            continue;
        }

        int fromX, fromY, toX, toY;
        if (input.length() != 4 ||
            !parseInput(input.substr(0, 2), fromX, fromY) ||
            !parseInput(input.substr(2, 2), toX, toY))
        {
            cout << "Invalid format! Use like 'a2a4'\n";
            continue;
        }

        Pieces *piece = board.getPiece(fromX, fromY);
        if (!piece)
        {
            cout << "No piece at " << input.substr(0, 2) << "!\n";
            continue;
        }

        if (board.movePiece(fromX, fromY, toX, toY)) {
            currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
        } else {
            cout << "Invalid move! ";
            if (board.isKingInCheck(currentTurn)) {
                cout << "Your king is still in check!\n";
            }
        }

        if (board.isGameOver()) {
            break;
        }
    }

    cout << "Game ended.\n";
    return 0;
}