#include <iostream>
#include <string>
#include "game_utils.h"


void playGame(Player& player1, Player& player2) {
    Board board = Board();
    
    Player* next_player = &player1;
    
    std::cout << "Welcome to tictactoe" << std::endl;
    std::cout << "Player " << player1.symbol << ", enter position <row,col>: ";
    board.printBoard();

    while (true) {
        std::cout << "Player " << next_player->symbol << ", enter position <row,col>: ";
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
    AIPlayer player1 = AIPlayer('X');
    HumanPlayer player2 = HumanPlayer('O');

    playGame(player1, player2);

}

