#include <SFML/Graphics.hpp>
#include <iostream>
#include "Morpion.h"

using namespace sf;
using namespace std;

const int WINDOW_SIZE = 300;
const int GRID_SIZE = 3;
const int CELL_SIZE = WINDOW_SIZE / GRID_SIZE;

int main() {
	TicTacToe game;
	bool gameOver = false;

	RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic Tac Toe");

	Color backgroundColor(255, 215, 0, 255);

	Font font;
	if (!font.loadFromFile("arial.ttf")) {
		cerr << "Failed to load font" << endl;
		return 1;
	}

	Text restartText;
	restartText.setFont(font);
	restartText.setCharacterSize(24);
	restartText.setFillColor(Color::Black);
	restartText.setString("Restart");

	FloatRect textRect = restartText.getLocalBounds();
	restartText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	restartText.setPosition(WINDOW_SIZE / 2.0f, WINDOW_SIZE / 2.0f);
	restartText.setStyle(Text::Bold);

	RectangleShape restartButton(Vector2f(100, 50));
	restartButton.setFillColor(backgroundColor);
	restartButton.setOutlineColor(Color::Black);
	restartButton.setOutlineThickness(2);
	restartButton.setPosition(WINDOW_SIZE / 2.0f, WINDOW_SIZE / 2.0f);
	restartButton.setOrigin(50, 25);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();

			if (!gameOver && event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					int row = event.mouseButton.y / CELL_SIZE;
					int col = event.mouseButton.x / CELL_SIZE;
					if (game.makeMove(row, col)) {
						cout << "Player " << int(game.getCurrentPlayer()) << " played at: " << row << ", " << col << endl;
						cout << "Checking for winner..." << endl;
						Player winner = game.checkWinner();
						if (winner != Player::None) {
							cout << "Player " << int(winner) << " wins!" << endl;
							gameOver = true;
						}
						else {
							// Check for draw
							bool isDraw = true;
							for (int i = 0; i < GRID_SIZE; ++i) {
								for (int j = 0; j < GRID_SIZE; ++j) {
									if (game.getCell(i, j) == Player::None) {
										isDraw = false;
										break;
									}
								}
							}
							if (isDraw) {
								cout << "It's a draw!" << endl;
								gameOver = true;
							}
						}
					}
				}
			}
			if (gameOver && event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
				game.reset();
				gameOver = false;
				cout << "Game reset!" << endl;
			}
		}

		window.clear(backgroundColor);

		if (gameOver) {
			window.draw(restartButton);
			window.draw(restartText);

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					if (restartText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						game.reset();
						gameOver = false;
						cout << "Game reset!" << endl;
					}
				}
			}
		}

		// Draw grid
		if (!gameOver) {
			RectangleShape line(Vector2f(WINDOW_SIZE - 1, 2));
			line.setFillColor(Color::Black);
			for (int i = 1; i < GRID_SIZE; ++i) {
				line.setPosition(0, i * CELL_SIZE);
				window.draw(line);
				line.setPosition(i * CELL_SIZE, 0);
				line.setRotation(90);
				window.draw(line);
				line.setRotation(0);
			}
		}

		// Draw X's and O's

		/*Font font;
		if (!font.loadFromFile("arial.ttf")) {
			cerr << "Failed to load font" << endl;
			return 1;
		}*/

		if (!gameOver) {
			for (int i = 0; i < GRID_SIZE; ++i) {
				for (int j = 0; j < GRID_SIZE; ++j) {
					Player player = game.getCell(i, j);
					if (player != Player::None) {
						Text text;
						text.setFont(font);
						text.setCharacterSize(CELL_SIZE - 10);
						text.setFillColor(Color::Black);
						text.setStyle(Text::Bold);
						text.setString((player == Player::X) ? "X" : "O");
						FloatRect textRect = text.getLocalBounds();
						text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
						text.setPosition(j * CELL_SIZE + CELL_SIZE / 2.0f, i * CELL_SIZE + CELL_SIZE / 2.0f);
						window.draw(text);
					}
				}
			}
		}

		window.display();
	}

	return 0;
}