#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Random.h"
#include <string>
#include <iostream>
#include "Board.h"
#include <fstream>
using namespace std;

int main()
{
    sf::Sprite Mine(TextureManager::GetTexture("mine"));
    sf::Sprite Flag(TextureManager::GetTexture("flag"));
    sf::Sprite FaceHappy(TextureManager::GetTexture("face_happy"));
    sf::Sprite FaceLose(TextureManager::GetTexture("face_lose"));
    sf::Sprite FaceWin(TextureManager::GetTexture("face_win"));
    sf::Sprite Test1(TextureManager::GetTexture("test_1"));
    sf::Sprite Test2(TextureManager::GetTexture("test_2"));
    sf::Sprite Test3(TextureManager::GetTexture("test_3"));
    sf::Sprite Debug(TextureManager::GetTexture("debug"));
    sf::Sprite Number1(TextureManager::GetTexture("number_1"));
    sf::Sprite Number2(TextureManager::GetTexture("number_2"));
    sf::Sprite Number3(TextureManager::GetTexture("number_3"));
    sf::Sprite Number4(TextureManager::GetTexture("number_4"));
    sf::Sprite Number5(TextureManager::GetTexture("number_5"));
    sf::Sprite Number6(TextureManager::GetTexture("number_6"));
    sf::Sprite Number7(TextureManager::GetTexture("number_7"));
    sf::Sprite Number8(TextureManager::GetTexture("number_8"));
    sf::Texture& digitsImage = TextureManager::GetTexture("digits");
    sf::Sprite digits[11];
    for (int i = 0; i < 11; ++i) {
        digits[i].setTexture(digitsImage);
        digits[i].setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
    }
    TextureManager::AllSprites();
    TextureManager::PrintTextureNames();
    int rows = 0;
    int cols = 0;
    int mines = 0;
    std::ifstream config("boards/config.cfg");
    config >> rows >> cols >> mines; // read board outside and pass in to be able to change boards later
    config.close();

    Board board(rows, cols, mines);
    
    //board.ResetBoard();
    board.revealAdjacentTiles();
    board.SetMines();
    board.calculateAdjacentMines();
    //Tile tile;
    
    sf::RenderWindow window(sf::VideoMode(rows * 32.0f, cols * 32.0f + 100), "Minesweeper", sf::Style::Close);
   //get mine count first if not 0 recursion
    // and do same thing with neibgotrs, when touches mine it stops ------------------------------------------------

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (board.gameOver) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        auto mousePosition = sf::Mouse::getPosition(window);
                        if (FaceHappy.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            board.ResetBoard();
                            board.remainingMines = mines;
                            board.SetMines();
                            board.calculateAdjacentMines();
                            //board.gameOver = false;
                        }
                        if (Test1.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            board.ResetBoard();
                            board.LoadTestBoard("testboard1");
                            board.calculateAdjacentMines();
                            board.revealAdjacentTiles();
                        }
                        if (Test2.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            cout << mousePosition.x << "," << mousePosition.y << endl;
                            board.ResetBoard();
                            board.LoadTestBoard("testboard2");
                            board.calculateAdjacentMines();
                            board.revealAdjacentTiles();
                        }
                        if (Test3.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            board.ResetBoard();
                            board.LoadTestBoard("testboard3");
                            board.calculateAdjacentMines();
                            board.revealAdjacentTiles();
                        }
                    }
                    continue;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto mousePosition = sf::Mouse::getPosition(window);
                    if (Test1.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        board.ResetBoard();
                        board.LoadTestBoard("testboard1");
                        board.calculateAdjacentMines();
                        board.revealAdjacentTiles();
                    }
                    if (Test2.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        cout << mousePosition.x << "," << mousePosition.y << endl;
                        board.ResetBoard();
                        board.LoadTestBoard("testboard2");
                        board.calculateAdjacentMines();
                        board.revealAdjacentTiles();
                    }
                    if (Test3.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        board.ResetBoard();
                        board.LoadTestBoard("testboard3");
                        board.calculateAdjacentMines();
                        board.revealAdjacentTiles();
                    }
                    if (Debug.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        board.ToggleMines();
                    }
                    if (FaceHappy.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        board.ResetBoard();
                        board.remainingMines = mines;
                        board.SetMines();
                        board.calculateAdjacentMines();
                    }
                    int x = event.mouseButton.x / 32;
                    int y = event.mouseButton.y / 32;
                    if (x >= 0 && x < rows && y >= 0 && y < cols) {
                        Tile& currentTile = board.getTile(x, y);
                        if (currentTile.isMine && !board.gameOver) {
                            board.RevealMines();
                        }
                        else {
                            board.revealAdjacentTiles();
                            currentTile.reveal();
                            if (board.WinCondition()) {
                               board.FlagRemainingMines();
                            }
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    int x = event.mouseButton.x / 32;
                    int y = event.mouseButton.y / 32;
                    if (x >= 0 && x < rows && y >= 0 && y < cols && board(x, y).isHidden) {
                        board.toggleFlag(x, y);                        
                    }   
                }
            }
        }
        //1. clear anything that was previously rendered do this only once
        window.clear();
      
        //2. draw anything that we want to appear on the screen
        for (int row = 0; row < board.rows; ++row) {
            for (int col = 0; col < board.cols; ++col) {
                window.draw(board.tiles[row][col].GetSprite());
                if (board.WinCondition()) {
                    FaceWin.setPosition(rows * 32 / 2, cols * 32 + 1);
                    window.draw(FaceWin);
                }
                else if (board.gameOver == true) {
                    FaceLose.setPosition(rows * 32 / 2, cols * 32 + 1);
                    window.draw(FaceLose);
                }
                else {
                    FaceHappy.setPosition(rows * 32 / 2, cols * 32 + 1);
                    window.draw(FaceHappy);
                }
                Test1.setPosition(rows * 32 / 2 + 6 * 32, cols * 32 + 1);
                window.draw(Test1);
                Test2.setPosition(rows * 32 / 2 + 8 * 32, cols * 32 + 1);
                Test3.setPosition(rows * 32 / 2 + 10 * 32, cols * 32 + 1);
                window.draw(Test2);
                window.draw(Test3);
                Debug.setPosition(rows * 32 / 2 + 4 * 32, cols * 32 + 1);
                window.draw(Debug);

                // functor
                if (board(row, col).isMine && board(row,col).isRevealed) {
                    Mine.setPosition(board(row, col).position);
                    window.draw(Mine);
                }
                else if (board(row, col).mineCount == 1 && board(row,col).isRevealed) {
                    Number1.setPosition(board(row, col).position);
                    window.draw(Number1);
                }
                else if (board(row, col).mineCount == 2 && board(row, col).isRevealed) {
                    Number2.setPosition(board(row, col).position);
                    window.draw(Number2);
                }
                else if (board(row, col).mineCount == 3 && board(row, col).isRevealed) {
                    Number3.setPosition(board(row, col).position);
                    window.draw(Number3);
                }
                else if (board(row, col).mineCount == 4 && board(row, col).isRevealed) {
                    Number4.setPosition(board(row, col).position);
                    window.draw(Number4);
                }
                else if (board(row, col).mineCount == 5 && board(row, col).isRevealed) {
                    Number5.setPosition(board(row, col).position);
                    window.draw(Number5);
                }
                else if (board(row, col).mineCount == 6 && board(row, col).isRevealed) {
                    Number6.setPosition(board(row, col).position);
                    window.draw(Number6);
                }
                else if (board(row, col).mineCount == 7 && board(row, col).isRevealed) {
                    Number7.setPosition(board(row, col).position);
                    window.draw(Number7);
                }
                else if (board(row, col).mineCount == 8 && board(row, col).isRevealed) {
                    Number8.setPosition(board(row, col).position);
                    window.draw(Number8);
                }
                if (board(row, col).isFlagged) {
                    Flag.setPosition(board(row, col).position);
                    window.draw(Flag);
                }
                int scoreRows = rows;
                int scoreCols = cols * 32 + 1;
                int score = board.remainingMines - board.flagCount;
                string number = to_string(abs(score));

                if (score < 0) {
                    int negativePosition = 10;
                    digits[negativePosition].setPosition(scoreRows, scoreCols);
                    window.draw(digits[negativePosition]);
                    scoreRows += 21;
                }
                for (char digitInput : number) {
                    int digit = digitInput - '0';
                    digits[digit].setPosition(scoreRows, scoreCols);
                    window.draw(digits[digit]);
                    scoreRows += 21;
                }
            }   
        }
        //3. copy anything that was drawn to the display device. do this only once
        window.display();
    }
    TextureManager::Clear();
    return 0;
}