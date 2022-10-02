#include <iostream>
#include <string>

struct Position {
    int row;
    int col;
};

void printBoard(char board[][3]) {
    std::cout << std::endl;
    for (int i = 0; i < 3; ++i) {

        for (int j = 0; j < 3; ++j){
                std::cout << " " << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool checkWin(char board[][3], char player) {
    return (
        (
            board[0][0] == player &&
            board[0][1] == player &&
            board[0][2] == player
        ) ||
        (
            board[1][0] == player &&
            board[1][1] == player &&
            board[1][2] == player
        ) ||
        (
            board[2][0] == player &&
            board[2][1] == player &&
            board[2][2] == player
        ) ||
        (
            board[0][0] == player &&
            board[1][0] == player &&
            board[2][0] == player
        ) ||
        (
            board[0][1] == player &&
            board[1][1] == player &&
            board[2][1] == player
        ) ||
        (
            board[0][2] == player &&
            board[1][2] == player &&
            board[2][2] == player
        ) ||
        (
            board[0][0] == player &&
            board[1][1] == player &&
            board[2][2] == player
        ) ||
        (
            board[0][2] == player &&
            board[1][1] == player &&
            board[2][0] == player
        )
    );
}

bool validateMove(char board[][3], Position position) {
    bool isValid = (
        position.row >= 0 && 
        position.col >= 0 &&
        position.row <= 2 &&
        position.col <= 2
        );
    bool isEmpty = board[position.row][position.col] == '-';

    return isValid && isEmpty;
}

bool isGameOver(char board[][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j){
            if (board[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}

Position getPosition() {
    std::string pos;
    std::cin >> pos;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int len = pos.length();
    int delim = pos.find(',');

    std::string pos1 = pos.substr(0, delim);
    std::string pos2 = pos.substr(delim+1, len-delim);
    
    Position position = {-1, -1};

    try {
        position.row = stoi(pos1);
        position.col = stoi(pos2);
    }
    catch (std::invalid_argument) {}
    
    return position;
}
