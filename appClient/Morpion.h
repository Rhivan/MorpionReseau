#pragma once

#include <SFML/Graphics.hpp>

enum class Player { None, X, O };

class TicTacToe {
private:
    Player grid[3][3];
    Player currentPlayer;

public:
    TicTacToe();
    void reset();
    bool makeMove(int row, int col);
    Player checkWinner() const;
    Player getCurrentPlayer() const;
    Player getCell(int row, int col) const;
};

