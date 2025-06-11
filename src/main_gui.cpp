#include "../include/chess_board.h"
#include "../include/pieces.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>

class ChessGUI {
private:
    sf::RenderWindow window;
    ChessBoard board;
    sf::Texture pieceTextures[12];
    sf::Sprite pieceSprites[8][8];
    sf::RectangleShape squares[8][8];
    sf::Font font;
    sf::Text turnText;
    sf::Text statusText;
    sf::RectangleShape restartButton;
    sf::Text restartButtonText;
    sf::RectangleShape banner;
    sf::Text bannerText;
    
    int selectedX = -1, selectedY = -1;
    bool gameOver = false;
    bool showBanner = false;
    
    // Colors
    sf::Color lightSquareColor = sf::Color(240, 217, 181);
    sf::Color darkSquareColor = sf::Color(181, 136, 99);
    sf::Color highlightColor = sf::Color(247, 247, 105, 150);
    sf::Color moveHintColor = sf::Color(106, 168, 79, 150);
    
    void loadTextures() {
const std::string pieceNames[12] = {
    "white_king", "white_queen", "white_rook", "white_bishop", "white_knight", "white_pawn",
    "black_king", "black_queen", "black_rook", "black_bishop", "black_knight", "black_pawn"
};

        for (int i = 0; i < 12; i++) {
            if (!pieceTextures[i].loadFromFile("bin/" + pieceNames[i] + ".png")) {
                std::cerr << "Failed to load texture: " << pieceNames[i] << ".png" << std::endl;
            }
        }
    }
    
    void initializeBoard() {
        const float squareSize = 64.0f;
        const float startX = (window.getSize().x - squareSize * 8) / 2;
        const float startY = (window.getSize().y - squareSize * 8) / 2;
        
        // Create squares
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                squares[x][y].setSize(sf::Vector2f(squareSize, squareSize));
                squares[x][y].setPosition(startX + y * squareSize, startY + x * squareSize);
                squares[x][y].setFillColor((x + y) % 2 ? darkSquareColor : lightSquareColor);
            }
        }
        
        // Initialize piece sprites
        updatePieceSprites();
        
        // Load font
        if (!font.loadFromFile("bin/arial.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
        }
        
        // Setup turn text
        turnText.setFont(font);
        turnText.setCharacterSize(24);
        turnText.setFillColor(sf::Color::White);
        turnText.setPosition(20, 20);
        updateTurnText();
        
        // Setup status text
        statusText.setFont(font);
        statusText.setCharacterSize(20);
        statusText.setFillColor(sf::Color::White);
        statusText.setPosition(20, 50);
        
        // Setup restart button
        restartButton.setSize(sf::Vector2f(150, 40));
        restartButton.setPosition(window.getSize().x - 170, 20);
        restartButton.setFillColor(sf::Color(70, 70, 70));
        restartButton.setOutlineThickness(2);
        restartButton.setOutlineColor(sf::Color::White);
        
        restartButtonText.setFont(font);
        restartButtonText.setString("Restart Game");
        restartButtonText.setCharacterSize(20);
        restartButtonText.setFillColor(sf::Color::White);
        restartButtonText.setPosition(
            restartButton.getPosition().x + (restartButton.getSize().x - restartButtonText.getLocalBounds().width) / 2,
            restartButton.getPosition().y + 8
        );
        
        // Setup banner
        banner.setSize(sf::Vector2f(400, 100));
        banner.setPosition((window.getSize().x - 400) / 2, (window.getSize().y - 100) / 2);
        banner.setFillColor(sf::Color(0, 0, 0, 200));
        banner.setOutlineThickness(3);
        banner.setOutlineColor(sf::Color::White);
        
        bannerText.setFont(font);
        bannerText.setCharacterSize(30);
        bannerText.setFillColor(sf::Color::White);
        bannerText.setPosition(
            banner.getPosition().x + (banner.getSize().x - bannerText.getLocalBounds().width) / 2,
            banner.getPosition().y + 30
        );
    }
    
    void updatePieceSprites() {
        const float squareSize = 64.0f;
        const float startX = (window.getSize().x - squareSize * 8) / 2;
        const float startY = (window.getSize().y - squareSize * 8) / 2;
        
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                Pieces* piece = board.getPiece(x, y);
                if (piece) {
                    int textureIndex = static_cast<int>(piece->getType()) + (piece->getColor() == Color::Black ? 6 : 0);
                    
                    pieceSprites[x][y].setTexture(pieceTextures[textureIndex]);
                    pieceSprites[x][y].setPosition(startX + y * squareSize, startY + x * squareSize);
                    
                    // Scale to fit square
                    sf::FloatRect bounds = pieceSprites[x][y].getLocalBounds();
                    float scale = squareSize / std::max(bounds.width, bounds.height);
                    pieceSprites[x][y].setScale(scale, scale);
                    
                    // Center in square
                    pieceSprites[x][y].setPosition(
                        startX + y * squareSize + (squareSize - bounds.width * scale) / 2,
                        startY + x * squareSize + (squareSize - bounds.height * scale) / 2
                    );
                }
            }
        }
    }
    
    void updateTurnText() {
        turnText.setString("Current Turn: " + std::string(board.getCurrentTurn() == Color::White ? "White" : "Black"));
    }
    
    void handleSquareClick(int x, int y) {
        if (gameOver) return;
        
        // Convert window coordinates to board coordinates
        const float squareSize = 64.0f;
        const float boardStartX = (window.getSize().x - squareSize * 8) / 2;
        const float boardStartY = (window.getSize().y - squareSize * 8) / 2;
        
        int boardX = (y - boardStartY) / squareSize;
        int boardY = (x - boardStartX) / squareSize;
        
        // Check if click is outside the board
        if (boardX < 0 || boardX >= 8 || boardY < 0 || boardY >= 8) {
            selectedX = -1;
            selectedY = -1;
            return;
        }
        
        // If no piece is selected, select a piece of the current turn's color
        if (selectedX == -1) {
            Pieces* piece = board.getPiece(boardX, boardY);
            if (piece && piece->getColor() == board.getCurrentTurn()) {
                selectedX = boardX;
                selectedY = boardY;
            }
        } 
        // If a piece is already selected, try to move it
        else {
            if (board.movePiece(selectedX, selectedY, boardX, boardY)) {
                updatePieceSprites();
                updateTurnText();
                
                // Check for game over
                if (board.isCheckmate(Color::White) || board.isCheckmate(Color::Black)) {
                    gameOver = true;
                    showBanner = true;
                    bannerText.setString(
                        (board.getCurrentTurn() == Color::White ? "Black" : "White") + 
                        std::string(" wins by checkmate!")
                    );
                    bannerText.setPosition(
                        banner.getPosition().x + (banner.getSize().x - bannerText.getLocalBounds().width) / 2,
                        banner.getPosition().y + 30
                    );
                } else if (board.isStalemate(board.getCurrentTurn())) {
                    gameOver = true;
                    showBanner = true;
                    bannerText.setString("Stalemate - Game drawn!");
                    bannerText.setPosition(
                        banner.getPosition().x + (banner.getSize().x - bannerText.getLocalBounds().width) / 2,
                        banner.getPosition().y + 30
                    );
                }
            }
            selectedX = -1;
            selectedY = -1;
        }
    }
    
    void restartGame() {
        board = ChessBoard(); // Reset the board
        selectedX = -1;
        selectedY = -1;
        gameOver = false;
        showBanner = false;
        updatePieceSprites();
        updateTurnText();
    }
    
    void draw() {
        window.clear(sf::Color(50, 50, 50));
        
        // Draw board squares
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                window.draw(squares[x][y]);
            }
        }
        
        // Highlight selected square
        if (selectedX != -1 && selectedY != -1) {
            sf::RectangleShape highlight = squares[selectedX][selectedY];
            highlight.setFillColor(highlightColor);
            window.draw(highlight);
            
            // Show possible moves (simplified - would need to call isMoveValid for all squares)
            // This is just a placeholder - a real implementation would need to calculate valid moves
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board.isMoveValid(selectedX, selectedY, x, y, board.getCurrentTurn())) {
                        sf::RectangleShape moveHint = squares[x][y];
                        moveHint.setFillColor(moveHintColor);
                        window.draw(moveHint);
                    }
                }
            }
        }
        
        // Draw pieces
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                if (board.getPiece(x, y)) {
                    window.draw(pieceSprites[x][y]);
                }
            }
        }
        
        // Draw coordinates
        sf::Text coordText;
        coordText.setFont(font);
        coordText.setCharacterSize(16);
        coordText.setFillColor(sf::Color::White);
        
        const float squareSize = 64.0f;
        const float boardStartX = (window.getSize().x - squareSize * 8) / 2;
        const float boardStartY = (window.getSize().y - squareSize * 8) / 2;
        
        // Draw file letters (a-h)
        for (int y = 0; y < 8; y++) {
            coordText.setString(std::string(1, 'a' + y));
            coordText.setPosition(boardStartX + y * squareSize + squareSize - 20, boardStartY + 8 * squareSize);
            window.draw(coordText);
        }
        
        // Draw rank numbers (1-8)
        for (int x = 0; x < 8; x++) {
            coordText.setString(std::to_string(8 - x));
            coordText.setPosition(boardStartX - 20, boardStartY + x * squareSize + 5);
            window.draw(coordText);
        }
        
        // Draw UI elements
        window.draw(turnText);
        window.draw(statusText);
        window.draw(restartButton);
        window.draw(restartButtonText);
        
        // Draw banner if game is over
        if (showBanner) {
            window.draw(banner);
            window.draw(bannerText);
        }
        
        window.display();
    }
    
public:
    ChessGUI() : window(sf::VideoMode(640, 640), "Chess Game") {
        loadTextures();
        initializeBoard();
    }
    
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        // Check if restart button was clicked
                        if (restartButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            restartGame();
                        } else {
                            handleSquareClick(event.mouseButton.x, event.mouseButton.y);
                        }
                    }
                }
            }
            
            draw();
        }
    }
};

int main() {
    ChessGUI gui;
    gui.run();
    return 0;
}