#include <iostream>
#include <string>
#include "game_utils.h"

int main() {
    char board[][3] = {{'-','-','-'}, {'-','-','-'}, {'-','-','-'}};
    // int len = sizeof(board)/sizeof(board[0]);
    std::cout << "Welcome to tictactoe" << std::endl;
    printBoard(board);
    
    char player = 'X';

    while (true) {
        std::cout << "Player " << player << ", enter position <row,col>: ";
        Position position = getPosition();
        if (validateMove(board, position) && position.row != -1) {
            board[position.row][position.col] = player;
            printBoard(board);

            if (checkWin(board, player)) {
                std::cout << "Player " << player << " wins!" << std::endl;
                break;
            }
            if (isGameOver(board)) {
                std::cout << "It's a draw!" << std::endl;
                break;
            }

            player = (player == 'X') ? 'O': 'X';
        } 
        else {
            std::cout << "Invalid move, please enter again" << std::endl;
        }
    }
}

