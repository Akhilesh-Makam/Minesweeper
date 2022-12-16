#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include "Tile.h"
#include "Board.h"
using namespace sf;
using namespace std;

int main()
{
	ifstream file("boards/config.cfg"); //read config file to get window measurements
	string temp;
	getline(file, temp);
	int columns = stoi(temp);
	getline(file, temp);
	int rows = stoi(temp);

	sf::RenderWindow window(sf::VideoMode(columns * 32, (rows*32)+100), "Minesweeper");
	

	Board board; //call game board

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			
			
			board.click(window); //sends update

			window.display();
		}


	}

	return 0;
}