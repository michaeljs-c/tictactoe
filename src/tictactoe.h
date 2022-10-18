#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  
#include <chrono>

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
            std::cout << " -----------" << std::endl;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j){
                    std::cout << "| " << board[i][j] << " ";
                }
                std::cout << "|" << std::endl;
                std::cout << " -----------" << std::endl;
            }
            std::cout << std::endl;
        }

        bool checkWin(char player) {
            return (
                // horizontal top
                (board[0][0] == player && board[0][1] == player && board[0][2] == player) ||
                // horizontal middle
                (board[1][0] == player && board[1][1] == player && board[1][2] == player) ||
                // horizontal top
                (board[2][0] == player && board[2][1] == player && board[2][2] == player) ||
                // vertical left
                (board[0][0] == player && board[1][0] == player && board[2][0] == player) ||
                // vertical middle
                (board[0][1] == player && board[1][1] == player && board[2][1] == player) ||
                // vertical right
                (board[0][2] == player && board[1][2] == player && board[2][2] == player) ||
                // diagonal right
                (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
                // diagonal left
                (board[0][2] == player && board[1][1] == player && board[2][0] == player)
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
                    else if (board[i][j] == 'O') {
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
        // start time
        auto start = std::chrono::high_resolution_clock::now();
        
        // minimax
        if (board.nextPlayer() == 'X') {
            min_max_value(board, "max", -100, 100);
        }
        else {
            min_max_value(board, "min", -100, 100);
        }
        
        // end time
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Time taken: " << (float)duration.count() / 1000000 << "s" << std::endl;

        return action;
    }

    int min_max_value(Board board, std::string min_or_max, int alpha, int beta) {
        int v = (min_or_max == "max")? -100 : 100;
        if (board.isGameOver()) return utility(board);
        
        Position best_position = Position();
        int best_value = v;

        for (Position position: board.getPositions()) {
            if (min_or_max == "max") {
                v = std::max(v, min_max_value(board.resultantBoard(position), "min", alpha, beta));    
                if (v > best_value) {
                    best_value = v;
                    best_position = position;
                }
                alpha = std::max(best_value, alpha);
                if (alpha >= beta) break;
            }
            else {
                v = std::min(v, min_max_value(board.resultantBoard(position), "max", alpha, beta));
                if (v < best_value) {
                    best_value = v;
                    best_position = position;
                }
                beta = std::min(best_value, beta);
                if (alpha >= beta) break;
            }   
        }
        if (best_position.row != -1) {
            this->action = best_position;
        }
        return v;
    }
};

class Player {
    public:
        char symbol;
        
        Player() {};
        Player(char symbol) {
            this->symbol = symbol;
        }
        virtual bool human_player() {
            return true;
        }
        virtual Position getPosition(Board board) {
            return Position();
        }
        
};

class AIPlayer: public Player {
    public:
        
        bool human_player() {
            return false;
        }
        Position getPosition(Board board) {
            return MiniMax().minimax(board);
        }
};

class HumanPlayer: public Player {
    public:
        
        bool human_player = {true};

        Position getUserPosition() {
            std::string pos;
            std::cin >> pos;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            int len = pos.length();
            int delim = pos.find(',');

            std::string pos1 = pos.substr(0, delim);
            std::string pos2 = pos.substr(delim+1, len-delim);
            
            Position position = Position();
            
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