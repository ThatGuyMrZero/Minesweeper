#pragma once
#include <vector>
#include <string>
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

class Tile {	
public:
	vector<Tile*> adjacentTiles;
	sf::Vector2f position;
	sf::Sprite tileHidden;
	sf::Sprite tileRevealed;
	sf::Sprite mineSprite;
	bool isHidden = true;
	bool isMine = false;
	bool isRevealed = false;
	bool isFlagged = false;
	int mineCount = 0;
	int flagCount = 0;
	Tile();
	Tile(float x, float y);
	void reveal();
	sf::Sprite& GetSprite();
	void toggleFlagged();
	void MineLocation();
};

class Board {
public:
	int rows = 0;
	int cols = 0;
	int mines = 0;
	int flagCount = 0;
	int remainingMines = 0;
	bool gameOver = false;
	int score = 0;
	vector<vector<Tile>> tiles;
	Board();
	Board(int rows, int cols, int mines);
	void SetMines();
	void RevealMines();
	void ToggleMines();
	void ResetBoard();
	void LoadTestBoard(const string& filename);
	void toggleFlag(int x, int y);
	void revealAdjacentTiles();
	void calculateAdjacentMines();
	bool WinCondition();
	void FlagRemainingMines();
	Tile& getTile(int x, int y);
	vector<vector<Tile>> operator()() {	
		return tiles;
	}
	Tile& operator()(int row, int col) {
		return tiles[row][col];
	}
};

