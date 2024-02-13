#include "Morpion.h"

TicTacToe::TicTacToe() : currentPlayer(Player::X) {
    reset();
}

void TicTacToe::reset() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            grid[i][j] = Player::None;
        }
    }
    currentPlayer = Player::X;
}

bool TicTacToe::makeMove(int row, int col) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
        return false;

    if (grid[row][col] == Player::None) {
        grid[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
        return true;
    }

    return false;
}

Player TicTacToe::checkWinner() const {
    for (int i = 0; i < 3; ++i) {
        // Check rows
        if (grid[i][0] != Player::None && grid[i][0] == grid[i][1] && grid[i][0] == grid[i][2])
            return grid[i][0];
        // Check columns
        if (grid[0][i] != Player::None && grid[0][i] == grid[1][i] && grid[0][i] == grid[2][i])
            return grid[0][i];
    }

    // Check diagonals
    if (grid[0][0] != Player::None && grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2])
        return grid[0][0];
    if (grid[0][2] != Player::None && grid[0][2] == grid[1][1] && grid[0][2] == grid[2][0])
        return grid[0][2];

    // Check for draw
    bool isDraw = true;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grid[i][j] == Player::None) {
                isDraw = false;
                break;
            }
        }
    }
    if (isDraw)
        return Player::None;

    return Player::None;
}

Player TicTacToe::getCurrentPlayer() const {
    return currentPlayer;
}

Player TicTacToe::getCell(int row, int col) const {
    return grid[row][col];
}
