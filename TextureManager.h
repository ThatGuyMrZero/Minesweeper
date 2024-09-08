#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>	
using namespace std;

class TextureManager {
public:
	static unordered_map<string, sf::Texture> textures;
	static void AllSprites();
	static void LoadTexture(string texturename);
	static sf::Texture& GetTexture(string texturename);
	static void Clear();
	static void PrintTextureNames();
};





