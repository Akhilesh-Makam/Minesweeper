#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
using namespace std;
using namespace sf;
#include "TextureManager.h"

class Tile {
public:
	bool isMine; //is tile a mine
	bool isFlagged; //was tile flagged
	bool isClicked; //was tile clicked
	bool debug; //is tile set to debug mode
	vector<Tile*> adjacentTiles; //surrounding tiles
	int xpos; //x coord of tile
	int ypos; //y coord of tile
	Sprite layer1; //when tile is not clicked
	Sprite layer2; //when tile is clicked
	Sprite image; //mine,number, or tile_revealed.
	Sprite flag; //flag icon

	Tile(float xpos_, float ypos_) {
		xpos = (int)xpos_;
		ypos = (int)ypos_;
		isMine = false; //set up of all basic info
		isFlagged = false;
		isClicked = false;
		debug = false;
		layer1.setTexture(TextureManager::GetTexture("tile_hidden")); 
		layer2.setTexture(TextureManager::GetTexture("tile_revealed"));
		flag.setTexture(TextureManager::GetTexture("flag"));
		flag.setColor(Color(255, 255, 255, 0)); //sets the flag sprite to see-through (opaque)
		layer1.setPosition(xpos_, ypos_); //gives each sprite the same position as tile
		layer2.setPosition(xpos_, ypos_);
		image.setPosition(xpos_, ypos_);
		flag.setPosition(xpos_, ypos_);
	}

	void setMine(bool mine) { //if tile is chosen as mine
		isMine = mine;
	}

	void changeFlag() { //if tile is flagged
		if (isClicked == false) { //tile has not been clicked
			if (isFlagged == false) { //flag tile
				isFlagged = true;
				flag.setColor(Color(255, 255, 255, 255)); //let player see flag
				return;
			}
			else { //unflag tile
				isFlagged = false;
				flag.setColor(Color(255, 255, 255, 0));
			}
		}
	}

	void update() { //when tile's image is being set
		if (isMine == true) { //assign image to mine
			image.setTexture(TextureManager::GetTexture("mine"));
		}
		else {
			int mines = 0;
			for (int i = 0; i < 8; i++) { //counts all the mines surrounding tile
				if (adjacentTiles[i] != nullptr && adjacentTiles[i]->isMine) {
					mines += 1;
				}
			}

			if (mines == 0) { //assigns number based on num of mines
				image.setTexture(TextureManager::GetTexture("tile_revealed"));
			}
			if (mines == 1) {
				image.setTexture(TextureManager::GetTexture("number_1"));
			}
			if (mines == 2) {
				image.setTexture(TextureManager::GetTexture("number_2"));
			}
			if (mines == 3) {
				image.setTexture(TextureManager::GetTexture("number_3"));
			}
			if (mines == 4) {
				image.setTexture(TextureManager::GetTexture("number_4"));
			}
			if (mines == 5) {
				image.setTexture(TextureManager::GetTexture("number_5"));
			}
			if (mines == 6) {
				image.setTexture(TextureManager::GetTexture("number_6"));
			}
			if (mines == 7) {
				image.setTexture(TextureManager::GetTexture("number_7"));
			}
			if (mines == 8) {
				image.setTexture(TextureManager::GetTexture("number_8"));
			}
		}
		//everything reset since this is called at beginning of new game or test
		isClicked = false;
		debug = false;
		isFlagged = false;
		image.setColor(Color(255, 255, 255, 0));
		flag.setColor(Color(255, 255, 255, 0));
		layer1.setColor(Color(255, 255, 255, 255));
	}

	void debugMode() { //sets tile w/ isMine = true to display mine
		if (isMine == true) {
			if (debug == true) {
				debug == false;
				image.setColor((Color(255, 255, 255, 0)));
			}
			else {
				debug = true;
				image.setColor(Color(255, 255, 255, 255));
			}
		}
	}

	void draw(RenderWindow& window) { //draws the sprites of each tile
		window.draw(layer2); //draw layer 2 on top of layer 1
		window.draw(layer1);
		window.draw(image);
		window.draw(flag);
	}

	bool clicked() { //when tile is clicked on
		if (isClicked == false && isFlagged == false) { //if tile has not been clicked yet and is not flagged
			isClicked = true;
			layer1.setColor(Color(255, 255, 255, 0));
			image.setColor(Color(255, 255, 255, 255)); //show image and layer 2 by removing layer 1

			int mines = 0;
			for (int i = 0; i < 8; i++) { //check how many mines surround tile
				if (adjacentTiles[i] != nullptr && adjacentTiles[i]->isMine) {
					mines += 1;
				}
			}

			if (mines == 0 && isMine == false) { //if none found and tile itself is not a mine
				for (int i = 0; i < adjacentTiles.size(); i++) {
					if (adjacentTiles[i] != nullptr && adjacentTiles[i]->isMine == false) { //reveal the next surrounding tile with same conditions through recursive function
						adjacentTiles[i]->clicked();
					}
				}
			}
		}
		return isMine; //return if the tile is a mine
	}
};