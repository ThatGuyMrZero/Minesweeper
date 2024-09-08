#include "TextureManager.h"
#include <string>
#include <iostream>
#include "Board.h"
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::AllSprites() {
    TextureManager::LoadTexture("debug");
    TextureManager::LoadTexture("digits");
    TextureManager::LoadTexture("face_happy");
    TextureManager::LoadTexture("face_lose");
    TextureManager::LoadTexture("face_win");
    TextureManager::LoadTexture("flag");
    TextureManager::LoadTexture("mine");
    for (int i = 1; i <= 3; ++i) {
        TextureManager::LoadTexture("test_" + to_string(i));
    }
    for (int i = 1; i <= 8; ++i) {
        TextureManager::LoadTexture("number_" + to_string(i));
    }
    TextureManager::LoadTexture("tile_hidden");
    TextureManager::LoadTexture("tile_revealed");
}

void TextureManager::LoadTexture(string filename) {
	string path = "images/";
	path += filename + ".png";
	textures[filename].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName) {
    if (textures.find(textureName) == textures.end()) {
        LoadTexture(textureName);
    }
    return textures[textureName];
}

void TextureManager::Clear() {
	textures.clear();
}

void TextureManager::PrintTextureNames() {
	for (const auto& pair : textures) {
		cout << "texture name: " << pair.first << endl;
	}
}



