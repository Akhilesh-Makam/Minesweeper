#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
using namespace std;
using namespace sf;
using std::unordered_map;
//this and cpp both from Fox's video

class TextureManager {
	//static == one and only one of these in memory, ever
	static unordered_map<string, sf::Texture> textures;
public:
	static void LoadTexture(string textureName);  //load texture based on name
	static sf::Texture& GetTexture(string textureName);  //return  texture based on nam
	static void Clear(); //call at end of main()
};
