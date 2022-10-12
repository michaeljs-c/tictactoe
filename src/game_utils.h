#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  

struct Position {
    int row;
    int col;

    Position(int row, int col){
        this->row = row;
        this->col = col;
    }
    Position() {
        this->row = -1;
        this->col = -1;
    }
    
};


class Board {
    public:
        std::vector<std::vector<char>> board = {{'-','-','-'},{'-','-','-'}, {'-','-','-'}};

        Board(Board& b) {
            this->board = b.board;
        }
        Board() {};

        Board(std::vector<std::vector<char>> board) {
            this->board = board;
        }

        Board resultantBoard(Position position) {
            Board new_board = Board(board);
            new_board.setPosition(position, nextPlayer());
            return new_board;
        }

        void setPosition(Position position, char symbol) {
            board[position.row][position.col] = symbol;
        }

        void printBoard() {
            std::cout << std::endl;
            for (int i = 0; i < 3; ++i) {

                for (int j = 0; j < 3; ++j){
                        std::cout << " " << board[i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        bool checkWin(char player) {
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

        bool validateMove(Position position) {
            bool isValid = (
                position.row >= 0 && 
                position.col >= 0 &&
                position.row <= 2 &&
                position.col <= 2
                );
            bool isEmpty = board[position.row][position.col] == '-';

            return isValid && isEmpty;
        }

        bool isGameOver() {
            if (checkWin('X') | checkWin('O')) {
                return true;
            }
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j){
                    if (board[i][j] == '-') {
                        return false;
                    }
                }
            }
            return true;
        }

        char nextPlayer() {
            int x_count = 0;
            int o_count = 0;

            for (int i=0; i<3; ++i) {
                for (int j=0; j<3; ++j) {
                    if (board[i][j] == 'X') {
                        ++x_count;

                    }
                    else if (board[i][j] == 'O')
                    {
                        ++o_count;
                    }
                }
            }
            return (x_count == o_count) ? 'X' : 'O';
        }

        std::vector<Position> getPositions() {
            std::vector<Position> moves = {};
            for (int i=0; i<3; ++i) {
                for (int j=0; j<3; ++j) {
                    if (board[i][j] == '-') {
                        moves.push_back(Position(i,j));
                    }
                }
            }
            return moves;
        }


};


class Player {
    public:
        char symbol;

        Player(char symbol) {
            this->symbol = symbol;
        }

        virtual Position getPosition(Board board) {
            Position position{-1,-1};
            return position;
        }
        
};

struct ActionValue {
    int value;
    Position position;

    ActionValue(int value, Position position) {
        this->value = value;
        this->position = position;
    }
};

class MiniMax {
    public:
        Position action = Position();

    int utility(Board board) {
        if (board.checkWin('X')) {
            return 1;
        }
        else if (board.checkWin('O')) {
            return -1;
        }
        else return 0;
    }

    Position minimax(Board board) {
        if (board.nextPlayer() == 'X') {
            max_value(board);
        }
        else {
            min_value(board);
        }
        return action;
    }

    int max_value(Board board) {
        int v = -100;
        if (board.isGameOver()){
            return utility(board);
        }
        ActionValue max_av = ActionValue(v, Position());

        for (Position position: board.getPositions()) {
            v = std::max(v, min_value(board.resultantBoard(position)));
            if (v > max_av.value) {
                max_av.value = v;
                max_av.position = position;
            }
        }
        if (max_av.position.row != -1) {
            this->action = max_av.position;
        }
        return v;
    }

    int min_value(Board board) {
        int v = 100;
        if (board.isGameOver()){
            return utility(board);
        }
        ActionValue min_av = ActionValue(v, Position());

        for (Position position: board.getPositions()) {
            v = std::min(v, max_value(board.resultantBoard(position)));
            if (v < min_av.value) {
                min_av.value = v;
                min_av.position = position;
            }
        }
        if (min_av.position.row != -1) {
            this->action = min_av.position;
        }
        return v;
    }

};

class AIPlayer: public Player {
    public:
        using Player::Player;

        Position getPosition(Board board) {
            Position pos = MiniMax().minimax(board);
            return pos;
        }
};

class HumanPlayer: public Player {
    public:
        using Player::Player;

        Position getUserPosition() {
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

        Position getPosition(Board board) {
            return getUserPosition();
        }
            
};