#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
using namespace std;
#include <ctime>
#include <cstdlib>
using namespace sf;
#include "TextureManager.h"
#include "Tile.h"
//links used for this project
/*
https://www.sfml-dev.org/tutorials/2.5/graphics-sprite.php#the-white-square-problem
https://www.simplilearn.com/tutorials/cpp-tutorial/random-number-generator-in-cpp#:~:text=How%20to%20Generate%20Random%20Numbers%20in%20C%2B%2B%20Within%20a%20Range,(rand()%20%25%20100).
https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Sprite.php#a3fefec419a4e6a90c0fd54c793d82ec2
https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Transformable.php#a4dbfb1a7c80688b0b4c477d706550208
https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Vector2.php#a58c32383b5291380db4b43a289f75988
https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
*/
class Board {
public:
	int numOfMines;
	int initialMines;
	int flagCount;
	int rows;
	int columns;
	vector<bool> mines;
	vector<Tile> tiles;
	vector<int> testcase1;
	vector<int> testcase2;
	vector<int> testcase3;
	Sprite debugImage;
	Sprite test1;
	Sprite test2;
	Sprite test3;
	Sprite restart;
	Sprite negCount;
	Sprite hundredCount;
	Sprite tenCount;
	Sprite oneCount;
	bool gameLost;
	bool gameDone;
	Board();
	void randomMines();
	void flagCounter();
	void setImages();
	void debug();
	void setTest(vector<int>& test);
	void draw(RenderWindow& w);
	void setRestart();
	void click(RenderWindow& w);
};

Board::Board() {
	srand((unsigned)time(NULL)); //resets randomizer each time
	ifstream file("boards/config.cfg"); //get info from config file and set to variables
	string temp;
	getline(file, temp);
	columns = stoi(temp);
	getline(file, temp);
	rows = stoi(temp);
	getline(file, temp);
	initialMines = stoi(temp);
	numOfMines = stoi(temp);
	flagCount = numOfMines;
	gameLost = false;
	gameDone = false;

	//
	ifstream file1("boards/testboard1.brd"); //store test board 1 info
	while (getline(file1, temp)) {
		for (int i = 0; i < temp.size(); i++) {
			char curr = temp[i];
			int tempp = curr - '0';
			testcase1.push_back(tempp);
		}
	}

	ifstream file2("boards/testboard2.brd"); //store test board 2 info
	while (getline(file2, temp)) {
		for (int i = 0; i < temp.size(); i++) {
			char curr = temp[i];
			int tempp = curr - '0';
			testcase2.push_back(tempp);
		}
	}

	ifstream file3("boards/testboard3.brd"); //store test board 3 info
	while (getline(file3, temp)) {
		for (int i = 0; i < temp.size(); i++) {
			char curr = temp[i];
			int tempp = curr - '0';
			testcase3.push_back(tempp);
		}
	}

	//set each sprite to its initial texture
	debugImage.setTexture(TextureManager::GetTexture("debug"));
	test1.setTexture(TextureManager::GetTexture("test_1"));
	test2.setTexture(TextureManager::GetTexture("test_2"));
	test3.setTexture(TextureManager::GetTexture("test_3"));
	restart.setTexture(TextureManager::GetTexture("face_happy"));
	negCount.setTexture(TextureManager::GetTexture("digits"));
	hundredCount.setTexture(TextureManager::GetTexture("digits"));
	tenCount.setTexture(TextureManager::GetTexture("digits"));
	oneCount.setTexture(TextureManager::GetTexture("digits"));
	negCount.setTextureRect(Rect<int>(21*11, 0, 21, 32));
	hundredCount.setTextureRect(Rect<int>(0, 0, 21, 32));
	tenCount.setTextureRect(Rect<int>(0, 0, 21, 32)); //
	oneCount.setTextureRect(Rect<int>(0, 0, 21, 32));
	negCount.setColor(Color(255, 255, 255, 0));

	//set sprite position based on columns and rows 
	negCount.setPosition(0, rows * 32);
	hundredCount.setPosition(21, rows * 32);
	tenCount.setPosition(42, rows * 32);
	oneCount.setPosition(63, rows * 32);
	restart.setPosition(200, rows * 32);
	debugImage.setPosition(400, rows * 32);
	test1.setPosition(464, rows * 32);
	test2.setPosition(528, rows * 32);
	test3.setPosition(592, rows * 32);

	//create columsn * rows tiles & set position accordingly
	float x = 0;
	float y = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Tile temp = Tile(x, y);
			tiles.push_back(temp);
			x += 32.0f;
		}
		x = 0;
		y += 32.0f;
	}

	//get which tiles will be mines
	randomMines();
}

void Board::randomMines() { //randomly choses #numOfMines random numbers between 0 - tiles.size()
	mines.clear();
	numOfMines = initialMines; //resets numOfMines whenever initial, or from test to regular game
	vector<int> indexes;
	for (int i = 0; i < numOfMines; i++) {
		int temp;
		bool found = false;
		while (found == false) {
			temp = 0 + (rand() % (rows * columns));
			if (find(indexes.begin(), indexes.end(), temp) == indexes.end()) { //checks if number was chosen already, if so randomize again
				indexes.push_back(temp);
				found = true;
			}
		}
	}
	for (int i = 0; i < tiles.size(); i++) {
		if (find(indexes.begin(), indexes.end(), i) != indexes.end()) { //assigns true/false based on if index is in indexes
			mines.push_back(true);
		}
		else {
			mines.push_back(false);
		}
	}

	for (int i = 0; i < tiles.size(); i++) { //sets mine based on indexes vector
		tiles[i].setMine(mines[i]);
	}
	flagCount = initialMines; //reset flag count
	flagCounter(); //resets flag count sprite
	setImages(); //sets images accordingly
}

void Board::flagCounter() {
	int temp = flagCount;
	if (flagCount < 100 && flagCount >= 0) {//sets sprites when flagcount <100
		int ones = temp % 10; 
		temp = temp / 10;
		int tens = temp % 10;
		negCount.setColor(Color(255, 255, 255, 0));
		hundredCount.setTextureRect(Rect<int>(0, 0, 21, 32)); //no digit needed, set to zero
		tenCount.setTextureRect(Rect<int>((21 * tens), 0, 21, 32)); //sets to remainder from second modulo
		oneCount.setTextureRect(Rect<int>((21 * ones), 0, 21, 32)); //sets to remainder from first modulo
	}
	else if (flagCount >= 100){
		int ones = temp % 10;
		temp = temp / 10;
		int tens = temp % 10;
		temp = temp / 10;
		int hundreds = temp % 10;
		negCount.setColor(Color(255, 255, 255, 0));
		hundredCount.setTextureRect(Rect<int>((21 * hundreds), 0, 21, 32)); //sets to remainder from third modulo
		tenCount.setTextureRect(Rect<int>((21 * tens), 0, 21, 32)); //sets to remainder from second module
		oneCount.setTextureRect(Rect<int>((21 * ones), 0, 21, 32)); //setes to remainder from third module
	}
	else if (flagCount < 0 && flagCount >= -99) {
		temp *= -1;
		int ones = temp % 10;
		temp = temp / 10;
		int tens = temp % 10;
		negCount.setColor(Color(255, 255, 255, 255));
		hundredCount.setTextureRect(Rect<int>(0, 0, 21, 32)); //no digit needed, set to zero
		tenCount.setTextureRect(Rect<int>((21 * tens), 0, 21, 32)); //sets to remainder from second modulo
		oneCount.setTextureRect(Rect<int>((21 * ones), 0, 21, 32)); //sets to remainder from first modulo
	}
	else if (flagCount <= -100) {
		temp *= -1;
		int ones = temp % 10;
		temp = temp / 10;
		int tens = temp % 10;
		temp = temp / 10;
		int hundreds = temp % 10;
		negCount.setColor(Color(255, 255, 255, 255));
		hundredCount.setTextureRect(Rect<int>((21 * hundreds), 0, 21, 32)); //sets to remainder from third modulo
		tenCount.setTextureRect(Rect<int>((21 * tens), 0, 21, 32)); //sets to remainder from second module
		oneCount.setTextureRect(Rect<int>((21 * ones), 0, 21, 32)); //setes to remainder from third module
	}
}

void Board::setTest(vector<int>& test) { //when test 1,2,3 called
	int tempMines = 0;
	mines.clear(); //clears mine vector
	for (int i = 0; i < test.size(); i++) { //checks how many mines in each test and loads into mine vector
		if (test[i] == 1) {
			mines.push_back(true);
			tempMines++;
		}
		else {
			mines.push_back(false);
		}
	}
	for (int i = 0; i < tiles.size(); i++) { //sets mines based on earlier for loop
		tiles[i].setMine(mines[i]);
	}
	numOfMines = tempMines; //reset numOfMines, flagcount, flagcounter, and if game is done and lost
	flagCount = numOfMines;
	setImages();
	flagCounter();
	gameDone = false;
	gameLost = false;
}

void Board::setImages() { //assigns image based on adjacentTiles
	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].adjacentTiles.clear(); //reset each tiles adjacentTiles vector

		for (int j = 0; j < 8; j++) {
			tiles[i].adjacentTiles.push_back(nullptr); //load it back up with nullptr
		}
		if (i == 0) { //tile is top left corner
			tiles[i].adjacentTiles[0] = &tiles[i + 1];
			tiles[i].adjacentTiles[1] = &tiles[i + columns];
			tiles[i].adjacentTiles[2] = &tiles[i + columns + 1];
		}
		else if (i == columns - 1) { //tile is top right corner
			tiles[i].adjacentTiles[0] = &tiles[i - 1];
			tiles[i].adjacentTiles[1] = &tiles[i + columns];
			tiles[i].adjacentTiles[2] = &tiles[i + columns - 1];
		}
		else if (i == (columns * (rows - 1))) { //tile is bottom left corner
			tiles[i].adjacentTiles[1] = &tiles[i + 1];
			tiles[i].adjacentTiles[2] = &tiles[i - columns];
			tiles[i].adjacentTiles[3] = &tiles[i - columns + 1];
		}
		else if (i == columns * rows - 1) { //tile is bottom right corner
			tiles[i].adjacentTiles[0] = &tiles[i - 1];
			tiles[i].adjacentTiles[1] = &tiles[i - columns];
			tiles[i].adjacentTiles[2] = &tiles[i - columns - 1];
		}
		else if (i < columns - 1) { //tile is in top row
			tiles[i].adjacentTiles[0] = &tiles[i - 1];
			tiles[i].adjacentTiles[1] = &tiles[i + 1];
			tiles[i].adjacentTiles[2] = &tiles[i + columns - 1];
			tiles[i].adjacentTiles[3] = &tiles[i + columns];
			tiles[i].adjacentTiles[4] = &tiles[i + columns + 1];
		}
		else if (i > columns * (rows - 1)) { //tile is in bottom row
			tiles[i].adjacentTiles[0] = &tiles[i - 1];
			tiles[i].adjacentTiles[1] = &tiles[i + 1];
			tiles[i].adjacentTiles[2] = &tiles[i - columns + 1];
			tiles[i].adjacentTiles[3] = &tiles[i - columns];
			tiles[i].adjacentTiles[4] = &tiles[i - columns - 1];
		}
		else if ((i % columns) == 0) { //tile is in left column
			tiles[i].adjacentTiles[0] = &tiles[i + 1];
			tiles[i].adjacentTiles[1] = &tiles[i + columns];
			tiles[i].adjacentTiles[2] = &tiles[i + columns + 1];
			tiles[i].adjacentTiles[3] = &tiles[i - columns];
			tiles[i].adjacentTiles[4] = &tiles[i - columns + 1];
		}
		else if (((i - (columns - 1)) % 25) == 0) { //tile is in right column
			tiles[i].adjacentTiles[0] = &tiles[i - 1];
			tiles[i].adjacentTiles[1] = &tiles[i - columns - 1];
			tiles[i].adjacentTiles[2] = &tiles[i - columns];
			tiles[i].adjacentTiles[3] = &tiles[i + (columns - 1)];
			tiles[i].adjacentTiles[4] = &tiles[i + columns];
		}
		else { //tile is in moddle
		tiles[i].adjacentTiles[0] = &tiles[i - 1];
		tiles[i].adjacentTiles[1] = &tiles[i + 1];
		tiles[i].adjacentTiles[2] = &tiles[i - (columns + 1)];
		tiles[i].adjacentTiles[3] = &tiles[i - (columns)];
		tiles[i].adjacentTiles[4] = &tiles[i - (columns - 1)];
		tiles[i].adjacentTiles[5] = &tiles[i + (columns - 1)];
		tiles[i].adjacentTiles[6] = &tiles[i + (columns)];
		tiles[i].adjacentTiles[7] = &tiles[i + (columns + 1)];
		}

		tiles[i].update(); //updates image based on passed section
	}
}



void Board::debug() { //sets tiles to debug
	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].debugMode();
	}
}

void Board::draw(RenderWindow& w) { //draws each tile's sprites and board's sprites
	for (int i = 0; i < tiles.size(); i++) {
		tiles[i].draw(w);
	}

	w.draw(debugImage);
	w.draw(test1);
	w.draw(test2);
	w.draw(test3);
	w.draw(restart);
	w.draw(hundredCount);
	w.draw(tenCount);
	w.draw(oneCount);
}

void Board::setRestart() { //whenever game is won, lost, or reset is pressed
	if (gameDone == true) {
		if (gameLost == true) {
			restart.setTexture(TextureManager::GetTexture("face_lose"));
			return;
		}
		else {
			restart.setTexture(TextureManager::GetTexture("face_win"));
			for (int i = 0; i < tiles.size(); i++) {
				if (tiles[i].isMine == true) {
					tiles[i].image.setColor(Color(255, 255, 255, 0));
				}
			}
			return;
		}
	}
	restart.setTexture(TextureManager::GetTexture("face_happy"));
}

//won't display until whole action is finished
void Board::click(RenderWindow& w) {
	if (Mouse::isButtonPressed(Mouse::Right) && gameDone == false) { //whenever right on mouse is clicked
		Vector2i interact = Mouse::getPosition(w);
		if (interact.x >= 0 && interact.x <= (columns * 32)) { //if right is clicked within tile field
			if (interact.y >= 0 && interact.y <= (rows * 32)) {
				int xCord = interact.x / 32; //get x-coordinate without sprite length
				int yCord = interact.y / 32; //get y-coordinate without sprite length
				int pos = (yCord * columns) + xCord; //this works for some math reason i just dont know why
				if (tiles[pos].isFlagged == false && tiles[pos].isClicked == false) { //if the clicked tile is not flagged, flagcount is > 0, and tile hasnt been clicked yet
					tiles[pos].changeFlag();
					flagCount--;
				}
				else if (tiles[pos].isClicked == false && tiles[pos].isFlagged == true) { //if tile hasnt been clicked but is flagged
					tiles[pos].changeFlag();
					flagCount++;
				}
				flagCounter(); //updates flag counter
			}
		}
	}

	if (Mouse::isButtonPressed(Mouse::Left)) { //if left mouse button is clicked
		Vector2i interact = Mouse::getPosition(w);
		if (interact.y >= 0 && interact.y < (rows*32) && gameDone == false) { //if left is clicked within tile field
			if (interact.x >= 0 && interact.x <= (columns * 32)) {
				int xCord = interact.x / 32; //get x-coordinate without sprite length
				int yCord = interact.y / 32; //get y-coordinate without sprite length
				int pos = (yCord * columns) + xCord; //this works for some math reason i just dont know why
				if (tiles[pos].clicked() == true) { //if tile is clicked is a mine
					gameDone = true; // game over and is lost
					gameLost = true;
					for (int i = 0; i < tiles.size(); i++) {
						if (tiles[i].isFlagged == true && tiles[i].isMine == true) {  //if the clicked tile is flagged, flagcount is > 0, and tile hasnt been clicked yet
							tiles[i].changeFlag(); //changes flagged tile with mine to remove flag
						}
						if (tiles[i].isMine == true) {
							tiles[i].clicked(); //reveal each mine
						}
					}
				}
			}

		}
		else if (interact.y >= (rows * 32) && interact.y < (rows * 32) + 60) { //below the tile region
			if (interact.x >= 200 && interact.x < 264) { //restart button
				gameDone = false; //restart everything
				gameLost = false;
				randomMines();
				setRestart();
				flagCounter();
			}
			if (interact.x >= 400 && interact.x < 464 && gameDone == false) { //activate debug mode
				debug();
			}
			if (interact.x >= 464 && interact.x < 528 && gameDone == false) { //set test case 1
				setTest(testcase1);
			}
			if (interact.x >= 528 && interact.x < 592 && gameDone == false) { //set test case 2
				setTest(testcase2);
			}
			if (interact.x >= 592 && interact.x < 656 && gameDone == false) { //set test case 3
				setTest(testcase3);
			}
		}
	}
	int temp = 0;
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].isMine == false && tiles[i].isClicked == true) {
			temp++;
		}
	}


	if (temp == (rows * columns - numOfMines) && gameLost == false) { //problem here
		gameDone = true;
		for (int i = 0; i < tiles.size(); i++) {
			if (tiles[i].isMine == true && tiles[i].isFlagged == false) {
				tiles[i].changeFlag();
				flagCount--;
			}
		}
		flagCounter();
	}

	setRestart();
	draw(w);

}

