#include <iostream>
#include <string>
#include "tictactoe.h"


void playGame(Player& player1, Player& player2) {
    Board board = Board();
    player1.symbol = 'X';
    player2.symbol = 'O';

    Player* next_player = &player1;
    
    std::cout << "Welcome to tictactoe" << std::endl;
    board.printBoard();

    while (true) {
        
        if (next_player->human_player()) {
            std::cout << "Player " << next_player->symbol << "'s turn. Enter position <row,col>: ";
        }
        else {
            std::cout << "Player " << next_player->symbol << "'s turn. Bot is thinking... beep boop" << std::endl;
        }
        Position position = next_player->getPosition(board);
        if (board.validateMove(position) && position.row != -1) {
            board.setPosition(position, next_player->symbol);
            board.printBoard();

            if (board.checkWin(next_player->symbol)) {
                std::cout << "Player " << next_player->symbol << " wins!" << std::endl;
                break;
            }
            else if (board.isGameOver()) {
                std::cout << "It's a draw!" << std::endl;
                break;
            }

            if (next_player->symbol == player1.symbol) {
                next_player = &player2;
            }
            else {
                next_player = &player1;
            }

        } 
        else {
            std::cout << "Invalid move, please enter again" << std::endl;
        }
    }
}

int main() {
    AIPlayer player1 = AIPlayer();
    HumanPlayer player2 = HumanPlayer();

    playGame(player1, player2);

    return 0;
}

