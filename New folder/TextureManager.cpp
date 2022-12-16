#include "TextureManager.h"

//redclare variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName) {
	string path = "images/" + fileName + ".png";

	textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName) {
	if (textures.find(textureName) == textures.end()) {
		//if texture DNE
		LoadTexture(textureName);
	}
	
	return textures[textureName];
}

void TextureManager::Clear() {
	
	textures.clear();
}
