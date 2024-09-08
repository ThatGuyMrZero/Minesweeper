#include "Board.h"
#include <fstream>
#include <string>
#include <iostream>
#include "Random.h"
using namespace std;

//TILE STUFF
Tile::Tile() {
	position = sf::Vector2f(0, 0);
}

Tile::Tile(float x, float y) {
	position = sf::Vector2f(x * 32, y * 32);	
	tileHidden.setTexture(TextureManager::GetTexture("tile_hidden"));
	tileRevealed.setTexture(TextureManager::GetTexture("tile_revealed"));
	mineSprite.setTexture(TextureManager::GetTexture("mine"));
	tileHidden.setPosition(position);
	tileRevealed.setPosition(position);
	mineSprite.setPosition(position);
}

void Tile::toggleFlagged() {
	if (!isRevealed) {
		isFlagged = !isFlagged;
	}
}

void Tile::MineLocation() {
	
	if (!isRevealed) {
		isMine = !isMine;
	}
 }

void Tile::reveal()
{
	if (!isFlagged && isHidden) {
		isHidden = false;
		isRevealed = true;
		if (mineCount == 0 && !isMine) {
			for (Tile* adjacentTile : adjacentTiles) {
				if (!adjacentTile->isRevealed && !adjacentTile->isMine) {
					adjacentTile->reveal();
				}
			}
		}
	}
}

sf::Sprite& Tile::GetSprite() {
	if (isHidden) {
		return tileHidden;
	}
	else {
		return tileRevealed;
	}
}

Board::Board() {}

void Board::FlagRemainingMines() {
	for (auto& row : tiles) {
		for (auto& tile : row) {
			if (tile.isMine && !tile.isFlagged) {
				tile.isFlagged = true;
				tile.flagCount++;
			}
		}
	}
	gameOver = true;
	remainingMines = 0;
}

bool Board::WinCondition() {
	for (auto& row : tiles) {
		for (auto& tile : row) {
			if (!tile.isMine && !tile.isRevealed) {
				return false;
			}
		}
	}
	return true;
}

void Board::ToggleMines() {
	for (auto& row : tiles) {
		for (auto& tile : row) {
			if (tile.isMine) {
				tile.isRevealed = !tile.isRevealed;
			}
		}
	}
}

void Board::RevealMines() {
	for (auto& row : tiles) {
		for (auto& tile : row) {
			if (tile.isMine) {
				tile.isRevealed = true;
				tile.isHidden = false;
			}
		}
	}
	gameOver = true;
}

void Board::toggleFlag(int x, int y) {
	Tile& tile = getTile(x, y);
	if (!tile.isRevealed || tile.isMine) {
		if (tile.isFlagged) {
			tile.isFlagged = false;
			flagCount--;  
		}
		else {
			tile.isFlagged = true;
			flagCount++; 
		}
	}
}

void Board::SetMines() {
	int count = 0;
	while (count < mines) {
		auto randRow = Random::Int(0, rows - 1);
		auto randCol = Random::Int(0, cols - 1);
		if (!tiles[randRow][randCol].isMine) {
			tiles[randRow][randCol].isMine = true;
			count++;
		}
	}
}

void Board::ResetBoard() {
	flagCount = 0;
	gameOver = false;
	remainingMines = 0;
	for (auto& row : tiles) {
		for (auto& tile : row) {
			tile.isMine = false;
			tile.isRevealed = false;
			tile.isFlagged = false;
			tile.isHidden = true;
			tile.mineCount = 0;		
		}
	}
}

void Board::LoadTestBoard(const string& filename) {
	ifstream file("boards/"+filename+".brd"); 
	string line;
	int colNum = 0; 
	while (getline(file, line) && colNum < cols) { 
		int length = line.size();
		for (int rowNum = 0; rowNum < rows; rowNum++) {
			if (rowNum < length) {
				bool isMine = (line[rowNum] == '1');
				tiles[rowNum][colNum].isMine = isMine;
				if (tiles[rowNum][colNum].isMine) {
					remainingMines++;
				}
			}
		}
		colNum++;  
	}	
}

Board::Board(int rows, int cols, int mines) : rows(rows), cols(cols), mines(mines) {
	remainingMines = mines;
	tiles.resize(rows);   
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			tiles[y].push_back(Tile(y, x));
		}
	}
}

Tile& Board::getTile(int x, int y) {
	return tiles[x][y];
}

void Board::revealAdjacentTiles() {	
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			Tile* currentTile = &tiles[row][col];
			currentTile->adjacentTiles.clear();			
			if (row > 0) { // top tile
				currentTile->adjacentTiles.push_back(&tiles[row - 1][col]);
			}
			if (row < rows - 1) { // bottom tile
				currentTile->adjacentTiles.push_back(&tiles[row + 1][col]);
			}
			if (col > 0) { // left tile
				currentTile->adjacentTiles.push_back(&tiles[row][col - 1]);
			}
			if (col < cols - 1) { // right tile
				currentTile->adjacentTiles.push_back(&tiles[row][col + 1]);
			}
			if (row > 0 && col > 0) { // diagonal up left
				currentTile->adjacentTiles.push_back(&tiles[row - 1][col - 1]);
			}
			if (row > 0 && col < cols - 1) { // diagonal up right
				currentTile->adjacentTiles.push_back(&tiles[row - 1][col + 1]);
			}
			if (row < rows - 1 && col > 0) { // diagonal down left
				currentTile->adjacentTiles.push_back(&tiles[row + 1][col - 1]);
			}
			if (row < rows - 1 && col < cols - 1) { // diagonal down right
				currentTile->adjacentTiles.push_back(&tiles[row + 1][col + 1]);
			}
		}
	}
}

void Board::calculateAdjacentMines() {
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			int boardmineCount = 0;
			for (Tile* tile : tiles[row][col].adjacentTiles) {
				if (tile->isMine) {
					boardmineCount++;
				}
			}
			tiles[row][col].mineCount = boardmineCount;
		}
	}
}







