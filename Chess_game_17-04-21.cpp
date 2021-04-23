// Object-oriented Chess game
//
// Final project for OOPC++ module
//
// Jason Dominguez ID:10154707 
// 25-03-21


#include <string>
#include <sstream>
#include <iostream>
#include <vector>


enum color {white, black};
std::string color_to_string(color chess_piece_color) {
    switch(chess_piece_color) {
        case white:
            return "white";
        case black:
            return "black";
        default:
            return "Invalid chess piece color";
    }
}
color string_to_color(std::string chess_piece_color) {
    if (chess_piece_color == "white" || chess_piece_color == "White") {
        return white;
    } else if (chess_piece_color == "black" || chess_piece_color == "Black") {
        return black;
    } else {
        std::cerr << "Invalid chess piece color";
        exit(-1);
    }
}
color opposite_color(color chess_piece_color) {
    if (chess_piece_color == white) {
        return black;
    } else {
        return white;
    }
}


int board_position_to_index(std::string position) {
    char row_letter = position[0];

    std::stringstream column_input;
    column_input << position[1];
    int column_number;
    column_input >> column_number;
    column_number -= 1;

    if (row_letter == 'A') return 0*8 + column_number;
    if (row_letter == 'B') return 1*8 + column_number;
    if (row_letter == 'C') return 2*8 + column_number;
    if (row_letter == 'D') return 3*8 + column_number;
    if (row_letter == 'E') return 4*8 + column_number;
    if (row_letter == 'F') return 5*8 + column_number;
    if (row_letter == 'G') return 6*8 + column_number;
    if (row_letter == 'H') return 7*8 + column_number;
    /*
    else {
        std::cerr << "Invalid board position.";
        exit(-1);
    }
    */
}
std::string board_index_to_position(int position_index) {
    int row_number = position_index/8 + 1;
    int column_number = position_index%8 + 1;

    std::string position;
    if (row_number == 1) position = "A";
    if (row_number == 2) position = "B";
    if (row_number == 3) position = "C";
    if (row_number == 4) position = "D";
    if (row_number == 5) position = "E";
    if (row_number == 6) position = "F";
    if (row_number == 7) position = "G";
    if (row_number == 8) position = "H";
    /*
    else {
        std::cerr << "Invalid board position.";
        exit(-1);
    }
    */

    std::stringstream position_column;
    position_column << column_number;
    std::string column_label;
    position_column >> column_label;

    position += column_label;

    return position;
}


class player
{
private:
    std::string name{};
    color piece_color{};

public:
    // Constructors
    player(std::string player_name, color player_color) :
           name{player_name}, piece_color{player_color} {}
    player(player &player_to_copy) {
        name = player_to_copy.name;
        piece_color = player_to_copy.piece_color;
    }
    // Destructor
    ~player(){}

    // Member functions
    std::string get_name() { return name; }
    color get_piece_color() { return piece_color; }
    player & operator=(player &player_to_copy) {
        if(&player_to_copy == this) return *this; // account for self-assignment
         
        name = player_to_copy.name; 
        piece_color = player_to_copy.piece_color;

        return *this;
    }
};

std::string ask_for_name() {
    std::cout << "Please enter your name: ";
    std::string name;
    std::cin >> name;

    return name;
}
color ask_for_color(std::string player_name) {
    std::cout << player_name << " please enter your chess piece color: ";
    std::string chess_piece_color;
    std::cin >> chess_piece_color;
    color piece_color = string_to_color(chess_piece_color);

    return piece_color;
}
void swap_player(player& current_player, player player_one, player player_two) {
    if (current_player.get_piece_color() == player_one.get_piece_color()) {
        current_player = player_two;
    } else {
        current_player = player_one;
    }
}


class board;

class chess_piece
{
    // Friend function for overloading the output operator
    friend std::ostream & operator<<(std::ostream &output, const chess_piece *piece);

protected:
    color piece_color{};
    int piece_id{};
    char piece_symbol{};

public:
    // Constructors
    chess_piece() {
        piece_color = white;
        piece_id = 0; // id of 0 used to denoted chess poece not present on board
        piece_symbol = ' ';
    }
    chess_piece(color white_or_black, int id, char symbol): 
        piece_color{white_or_black}, piece_id{id}, piece_symbol{symbol} {}
    // Destructor
    virtual ~chess_piece(){}

    // Member functions
    color get_piece_color() { return piece_color; }
    color get_piece_color() const { return piece_color; }
    int get_id() { return piece_id; }
    int get_id() const { return piece_id; }

    chess_piece & operator=(chess_piece &chess_piece_to_copy) {
        if (&chess_piece_to_copy == this) return *this; // account for self-assignment
        
        piece_color = chess_piece_to_copy.piece_color;
        piece_id = chess_piece_to_copy.piece_id;

        return *this;
    }
    virtual std::vector<int> get_valid_moves(int start_position, const board& chess_board)=0;
};

std::ostream & operator<<(std::ostream &output, const chess_piece *piece) {
    output << piece->piece_symbol;
    return output;
}

class pawn : public chess_piece
{
private:
    bool has_moved{false};
    
public:
    // Constructors
    pawn() : chess_piece() {}
    pawn(color white_or_black, int id) : chess_piece(white_or_black, id, 'p') {}
    // Destructor
    virtual ~pawn(){}

    // Member functions
    std::vector<int> get_valid_moves(int start_position, const board& chess_board);

};

class rook : public chess_piece
{  
public:
    // Constructors
    rook() : chess_piece() {}
    rook(color white_or_black, int id) : chess_piece(white_or_black, id, 'R') {}
    // Destructor
    virtual ~rook(){}

    // Member functions
    std::vector<int> get_valid_moves(int start_position, const board& chess_board);
};

class knight : public chess_piece
{
public:
    // Constructors
    knight() : chess_piece() {}
    knight(color white_or_black, int id) : chess_piece(white_or_black, id, 'N') {}
    // Destructor
    virtual ~knight(){}

    std::vector<int> get_valid_moves(int start_position, const board& chess_board);
};

class bishop : public chess_piece
{

public:
    // Constructors
    bishop() : chess_piece() {}
    bishop(color white_or_black, int id) : chess_piece(white_or_black, id, 'B') {}
    // Destructor
    virtual ~bishop(){}

    std::vector<int> get_valid_moves(int start_position, const board& chess_board);
};

class queen : public chess_piece
{

public:
    // Constructors
    queen() : chess_piece() {}
    queen(color white_or_black, int id) : chess_piece(white_or_black, id, 'Q') {}
    // Destructor
    virtual ~queen(){}

    std::vector<int> get_valid_moves(int start_position, const board& chess_board);
};

class king : public chess_piece
{
private:
    bool has_moved{false};
    
public:
    // Constructors
    king() : chess_piece() {}
    king(color white_or_black, int id) : chess_piece(white_or_black, id, 'K') {}
    // Destructor
    virtual ~king(){}

    std::vector<int> get_valid_moves(int start_position, const board& chess_board);
};


class board
{
    // Friend function for overloading the output operator
    friend std::ostream & operator<<(std::ostream &output, const board &b);

private:
    chess_piece **chess_board = new chess_piece*[8*8]; //8x8 chess board
    std::vector<chess_piece*> removed_chess_pieces;

public:
    board() {
        // Initialize white pieces on chess board
        // starting from bottom left to top right
        chess_board[0] = new rook(white, 1);
        chess_board[1] = new knight(white, 1);
        chess_board[2] = new bishop(white, 1);
        chess_board[3] = new king(white, 1);
        chess_board[4] = new queen(white, 1);
        chess_board[5] = new bishop(white, 2);
        chess_board[6] = new knight(white, 2);
        chess_board[7] = new rook(white, 2);

        chess_board[1*8] = new pawn(white, 1);
        chess_board[1*8 + 1] = new pawn(white, 2);
        chess_board[1*8 + 2] = new pawn(white, 3);
        chess_board[1*8 + 3] = new pawn(white, 4);
        chess_board[1*8 + 4] = new pawn(white, 5);
        chess_board[1*8 + 5] = new pawn(white, 6);
        chess_board[1*8 + 6] = new pawn(white, 7);
        chess_board[1*8 + 7] = new pawn(white, 8);

        // Initialize unoccupied chess board squares
        // rows 3,4,5,6 (array elements 16-48)
        for (size_t i{2*8} ; i < 6*8 ; i++) {
            chess_board[i] = nullptr; 
        }

        // Initialize black pieces on chess board
        chess_board[6*8] = new pawn(black, 1);
        chess_board[6*8 + 1] = new pawn(black, 2);
        chess_board[6*8 + 2] = new pawn(black, 3);
        chess_board[6*8 + 3] = new pawn(black, 4);
        chess_board[6*8 + 4] = new pawn(black, 5);
        chess_board[6*8 + 5] = new pawn(black, 6);
        chess_board[6*8 + 6] = new pawn(black, 7);
        chess_board[6*8 + 7] = new pawn(black, 8);
        
        chess_board[7*8] = new rook(black, 1);
        chess_board[7*8 + 1] = new knight(black, 1);
        chess_board[7*8 + 2] = new bishop(black, 1);
        chess_board[7*8 + 3] = new king(black, 1);
        chess_board[7*8 + 4] = new queen(black, 1);
        chess_board[7*8 + 5] = new bishop(black, 2);
        chess_board[7*8 + 6] = new knight(black, 2);
        chess_board[7*8 + 7] = new rook(black, 2);
    }
    ~board() {
        for (int i{0} ; i < 8 ; i++) {
            for (int j{0} ; j < 8 ; j++) {
                delete chess_board[i*8 + j];
            }
        }
        delete[] chess_board;
    }


    void move_piece(int initial_position, int final_position) {
        if (!chess_board[final_position]) {
            // Final position on chess board is empty
            chess_board[final_position] = &*chess_board[initial_position];
        } else {
            // Final position has a chess piece already there
            // so remove it from the board
            removed_chess_pieces.push_back(chess_board[final_position]);
            chess_board[final_position] = &*chess_board[initial_position];
        }
        // Make sure that the initial position on the board is now empty
        chess_board[initial_position] = nullptr;
    }

    chess_piece* operator[](int idx) const { 
        // Check for out of bound index
        if (idx > 63 && idx >= 0) {
            std::cerr << "Board index out of range, exiting.";
            exit(-1); 
        }
        return chess_board[idx];
    }
};

std::ostream & operator<<(std::ostream &output, const board &b) {
    std::string row_letters{"ABCDEFGH"};
    for (int i{0} ; i < 8 ; i++) {
        output << row_letters[i] << "||";
        for (int j{0} ; j < 8 ; j++) {
            if (j < 7) {
                if (b.chess_board[i*8 + j]) {
                    output << " " << b.chess_board[i*8 + j] << " |";
                }
                else {
                    output << "   |";
                }
            } else {
                if (b.chess_board[i*8 + j]) {
                    output << " " << b.chess_board[i*8 + j] << " ||" << std::endl;
                }
                else {
                    output << "   ||" << std::endl;
                }
            }
        }
        output << " -----------------------------------" << std::endl;
    }
    output << "    1   2   3   4   5   6   7   8  " << std::endl;

    return output;
}

void player_turn(player chess_player, board& chess_board) {
    std::cout << chess_player.get_name() << "'s turn..." << std::endl;

    // Initialize to a poistion on chess board with opposite color of player
    int start_position_index{};
    if (chess_player.get_piece_color() == white) {
        start_position_index = 63;
    } else {
        start_position_index = 0;
    }
    
    while (true) {
        std::vector<int> possible_moves{};
        while (possible_moves.size() == 0) {
            while (chess_board[start_position_index]->get_piece_color() != chess_player.get_piece_color()) {
                start_position_index = 16; // Set to a position with no chess piece
                while (!chess_board[start_position_index]) {
                    std::cout << "Please enter the position of the chess piece you want to move: " ;
                    std::string start_position;
                    std::cin >> start_position;
                    start_position_index = board_position_to_index(start_position);

                    if (!chess_board[start_position_index]) {
                        std::cout << "Ummm... that's... not a chess piece...... Please try again..." << std::endl;
                    }
                }
                if (chess_board[start_position_index]->get_piece_color() != chess_player.get_piece_color()) {
                    std::cout << "You can only move a " << color_to_string(chess_player.get_piece_color()) << " chess piece. You're not trying to cheat are you...?" << std::endl;
                }
            }
            // Get the possible moves for the chosen piece
            possible_moves = chess_board[start_position_index]->get_valid_moves(start_position_index, chess_board);
            if (possible_moves.size() == 0) {
                std::cout << "Sorry, this piece has no possible moves. Please try again..." << std::endl;
                if (chess_player.get_piece_color() == white) {
                    start_position_index = 63;
                } else {
                    start_position_index = 0;
                }
            }
        }

        std::cout << "Here are the possible moves for this chess piece: " << std::endl;
        std::vector<int>::iterator vector_begin{possible_moves.begin()};
        std::vector<int>::iterator vector_end{possible_moves.end()};
        std::vector<int>::iterator vector_iterator;
        for (vector_iterator = vector_begin ; vector_iterator < vector_end ; ++vector_iterator) {
            std::cout << board_index_to_position(*vector_iterator) << std::endl;
        }
    
        std::cout << "Do you still want to move this chess piece (y/n)?" << std::endl;
        std::string piece_to_move_decision;
        std::cin >> piece_to_move_decision;
        if (piece_to_move_decision == "y") {
            break;
        } else {
            if (chess_player.get_piece_color() == white) {
                start_position_index = 63;
            } else {
                start_position_index = 0;
            }
        }
    }

    std::cout << "To which of these positions do you want to move you piece?" << std::endl;
    std::string end_position;
    std::cin >> end_position;
    int end_position_index = board_position_to_index(end_position);

    chess_board.move_piece(start_position_index, end_position_index);
}


// Define functions to get valid move for chess pieces after defining the board class
std::vector<int> pawn::get_valid_moves(int start_position, const board& chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};
        
    // White pieces will move up the board (positive increment in position)
    // black pieces will move down (negative increment of position)
    int position_increment{};
    if (this->get_piece_color() == white) {
        position_increment = 1;
    } else {
        position_increment = -1;
    }
        
    std::vector<int> valid_new_positions;

    // Check if pawn can move forward by 1
    if (start_position_row + position_increment < 8) {
        if (!chess_board[start_position + 8*position_increment]) {
            valid_new_positions.push_back(start_position + 8*position_increment); // moving forward by 1
        }
    }
    // Check if pawn can move forward by 2 (if pawn's first move)
    if (!has_moved) {
        if (start_position_row + 2*position_increment < 8) {
            if (!chess_board[start_position + 2*8*position_increment]) {
                valid_new_positions.push_back(start_position + 2*8*position_increment); // moving forward by 2
            }
        }
    }
    // Check if diagonal moves are possible (if attacking opposite color piece)
    if (start_position_row + position_increment < 8 && start_position_col + position_increment < 8) {
        if (chess_board[start_position + (8*position_increment + position_increment)] && chess_board[start_position + (8*position_increment + position_increment)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + (8*position_increment + position_increment)); // moving forward and right by 1
        }
    }
    if (start_position_row + position_increment < 8 && start_position_col - position_increment >= 0) {
        if (chess_board[start_position + (8*position_increment - position_increment)] && chess_board[start_position + (8*position_increment - position_increment)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + (8*position_increment - position_increment)); // moving forward and left by 1
        }
    }

    return valid_new_positions;
}

std::vector<int> rook::get_valid_moves(int start_position, const board& chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Rooks can move vertically or horizontally any number of spaces, but cannot
    // jump over any other chess pieces

    // Find valid horizontal moves
    for (int i{start_position_col + 1} ; i < 8 ; i++) {
        if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() == this->get_piece_color()) {
            break;
        } else {
            valid_new_positions.push_back(8*start_position_row + i); // Move right
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }
    for (int i{start_position_col - 1} ; i >= 0 ; i--) {
        if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() == this->get_piece_color()) {
            break;
        } else {
            valid_new_positions.push_back(8*start_position_row + i); // Move left
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }
    // Find valid vertical moves
    for (int i{start_position_row + 1} ; i < 8 ; i++) {
        if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() == this->get_piece_color()) {
            break;
        } else {
            valid_new_positions.push_back(8*i + start_position_col); // Move forward
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }
    for (int i{start_position_row - 1} ; i >= 0 ; i--) {
        if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() == this->get_piece_color()) {
            break;
        } else {
            valid_new_positions.push_back(8*i + start_position_col); // Move backward
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }

    return valid_new_positions;
}

std::vector<int> knight::get_valid_moves(int start_position, const board& chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};
        
    std::vector<int> valid_new_positions;

    // Knights can move in L-shapes (e.g. 2 forward 1 left)

    // Check possible L-shape moves
    // Check 2 forward 1 right and 2 forward 1 left
    if (start_position_row + 2 < 8) {
        if (start_position_col + 1 < 8) {
            if (!chess_board[start_position + (2*8 + 1)] || chess_board[start_position + (2*8 + 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (2*8 + 1)); // move forward 2 and right 1
            }
        }
        if (start_position_col - 1 >= 0) {
            if (!chess_board[start_position + (2*8 - 1)] || chess_board[start_position + (2*8 - 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (2*8 - 1)); // move forward 2 and left 1
            }
        }
    }
    // Check 1 forward 2 right and 1 forward 2 left
    if (start_position_row + 1 < 8) {
        if (start_position_col + 2 < 8) {
            if (!chess_board[start_position + (8 + 2)] || chess_board[start_position + (8 + 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (8 + 2)); // move forward 1 and right 2
            }
        }
        if (start_position_col - 2 >= 0) {
            if (!chess_board[start_position + (8 - 2)] || chess_board[start_position + (8 - 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (8 - 2)); // move forward 1 and left 2
            }
        }
    }
    // Check 1 backward 2 right and 1 backward 2 left
    if (start_position_row - 1 >= 0) {
        if (start_position_col + 2 < 8) {
            if (!chess_board[start_position + (-8 + 2)] || chess_board[start_position + (-8 + 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (8 + 2)); // move forward 1 and right 2
            }
        }
        if (start_position_col - 2 >= 0) {
            if (!chess_board[start_position + (-8 - 2)] || chess_board[start_position + (-8 - 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-8 - 2)); // move forward 1 and left 2
            }
        }
    }
    // Check 2 backward 1 right and 2 backward 1 left
    if (start_position_row - 2 >= 0) {
        if (start_position_col + 1 < 8) {
            if (!chess_board[start_position + (-2*8 + 1)] || chess_board[start_position + (-2*8 + 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-2*8 + 1)); // move forward 2 and right 1
            }
        }
        if (start_position_col - 1 >= 0) {
            if (!chess_board[start_position + (-2*8 - 1)] || chess_board[start_position + (-2*8 - 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-2*8 - 1)); // move forward 2 and left 1
            }
        }
    }

    return valid_new_positions;
}

std::vector<int> bishop::get_valid_moves(int start_position, const board& chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Bishops can move diagonally any number of spaces, but cannot
    // jump over any other chess pieces

    // Find valid diagonal moves
    // Moving up and to the right
    bool is_blocked = false;
    for (int i{start_position_col + 1} ; i < 8 ; i++) {
        for (int j{start_position_row + 1} ; j < 8 ; j++) {
            if ( (start_position_col - i) == (start_position_row - j) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
        if (is_blocked) {
            break;
        }
    }
    // Moving up and to the left
    is_blocked = false;
    for (int i{start_position_col - 1} ; i >= 0 ; i--) {
        for (int j{start_position_row + 1} ; j < 8 ; j++) {
            if ( (i - start_position_col) == (start_position_row - j) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
        if (is_blocked) {
            break;
        }
    }
    // Moving down and to the right
    is_blocked = false;
    for (int i{start_position_col + 1} ; i < 8 ; i++) {
        for (int j{start_position_row - 1} ; j >= 0 ; j--) {
            if ( (start_position_col - i) == (j - start_position_row) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
        if (is_blocked) {
            break;
        }
    }
    // Moving down and to the left
    is_blocked = false;
    for (int i{start_position_col - 1} ; i >= 0 ; i--) {
        for (int j{start_position_row - 1} ; j >= 0 ; j--) {
            if ( (i - start_position_col) == (j - start_position_row) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
        if (is_blocked) {
            break;
        }
    }

    return valid_new_positions;
}

std::vector<int> queen::get_valid_moves(int start_position, const board& chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Queens can move diagonally, vertically or horizontally any number
    // of spaces, but cannot jump over any other chess pieces

    // Find valid horizontal moves
    for (int i{start_position_col + 1} ; i < 8 ; i++) {
        if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() == this->get_piece_color()) {
            continue;
        } else {
            valid_new_positions.push_back(8*start_position_row + i);
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }
    for (int i{start_position_col - 1} ; i >= 0 ; i--) {
        if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() == this->get_piece_color()) {
            continue;
        } else {
            valid_new_positions.push_back(8*start_position_row + i);
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*start_position_row + i] && chess_board[8*start_position_row + i]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }
    // Find valid vertical moves
    for (int i{start_position_row + 1} ; i < 8 ; i++) {
        if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() == this->get_piece_color()) {
            continue;
        } else {
            valid_new_positions.push_back(8*i + start_position_col);
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }
    for (int i{start_position_row - 1} ; i >= 0 ; i--) {
        if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() == this->get_piece_color()) {
            continue;
        } else {
            valid_new_positions.push_back(8*i + start_position_col);
            // If space contains opposite color piece, it cannot be jumped over
            if (chess_board[8*i + start_position_col] && chess_board[8*i + start_position_col]->get_piece_color() != this->get_piece_color()) {
                break;
            }
        }
    }

    // Find valid diagonal moves
    // Moving up and to the right
    for (int i{start_position_col + 1} ; i < 8 ; i++) {
        for (int j{start_position_row + 1} ; j < 8 ; j++) {
            if ( (start_position_col - i) == (start_position_row - j) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
    }
    // Moving up and to the left
    for (int i{start_position_col - 1} ; i >= 0 ; i--) {
        for (int j{start_position_row + 1} ; j < 8 ; j++) {
            if ( (i - start_position_col) == (start_position_row - j) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
    }
    // Moving down and to the right
    for (int i{start_position_col + 1} ; i < 8 ; i++) {
        for (int j{start_position_row - 1} ; j >= 0 ; j--) {
            if ( (start_position_col - i) == (j - start_position_row) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
    }
    // Moving down and to the left
    for (int i{start_position_col - 1} ; i >= 0 ; i--) {
        for (int j{start_position_row - 1} ; j >= 0 ; j--) {
            if ( (i - start_position_col) == (j - start_position_row) ) {
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
                        break;
                    }
                }
            } else {
                continue;
            }
        }
    }
    
    return valid_new_positions;
}

std::vector<int> king::get_valid_moves(int start_position, const board& chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};
        
    std::vector<int> valid_new_positions;

    // King can move in any direction by 1 only

    // Check if king can move forward by 1
    if (start_position_row + 1 < 8) {
        if (!chess_board[start_position + 8] || chess_board[start_position + 8]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + 8); // move forward by 1
        }
    }
    // Check if king can move backwards by 1
    if (start_position_row - 1 >= 0) {
        if (!chess_board[start_position - 8] || chess_board[start_position - 8]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position - 8); // move backward by 1
        }
    }
    // Check if king can move right by 1
    if (start_position_col + 1 < 8) {
        if (!chess_board[start_position + 1] || chess_board[start_position + 1]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + 1); // move right by 1
        }
    }
    // Check if king can move left by 1
    if (start_position_col - 1 >= 0) {
        if (!chess_board[start_position - 1] || chess_board[start_position - 1]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position - 1); // move left by 1
        }
    }
    // Check if diagonal moves are possible
    if (start_position_row + 1 < 8 && start_position_col + 1 < 8) {
        if (!chess_board[start_position + (8 + 1)] && chess_board[start_position + (8 + 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + (8 + 1)); // move forward and right by 1
        }
    }
    if (start_position_row + 1 < 8 && start_position_col - 1 >= 0) {
        if (!chess_board[start_position + (8 - 1)] && chess_board[start_position + (8 - 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + (8 - 1)); // move forward and left by 1
        }
    }
    if (start_position_row - 1 >= 0 && start_position_col + 1 < 8) {
        if (!chess_board[start_position - (8 - 1)] && chess_board[start_position - (8 - 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position - (8 - 1)); // move backwards and right by 1
        }
    }
    if (start_position_row - 1 >= 0 && start_position_col - 1 >= 0) {
        if (!chess_board[start_position - (8 + 1)] && chess_board[start_position - (8 + 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position - (8 + 1)); // moving backwards and left by 1
        }
    }

    return valid_new_positions;
}


int main() {
    
    std::cout << "Please enter the number of players (1/2): ";
    int number_of_players;
    std::cin >> number_of_players;

    if (number_of_players == 1) {
        std::cout << "AI has not yet been developed" << std::endl;
    } else if (number_of_players == 2) {
        std::cout << std::endl << "Player 1" << std::endl;
        std::string player_one_name = ask_for_name();
        color player_one_color = ask_for_color(player_one_name);
        player player_one(player_one_name, player_one_color);
        std::cout << player_one.get_name() << " your color is " << color_to_string(player_one.get_piece_color()) << std::endl;

        std::cout << std::endl << "Player 2" << std::endl;
        std::string player_two_name = ask_for_name();
        color player_two_color = opposite_color(player_one_color);
        player player_two(player_two_name, player_two_color);
        std::cout << player_two.get_name() << " your color is " << color_to_string(player_two.get_piece_color()) << std::endl;

        board chess_board;
        std::cout << chess_board << std::endl;

        bool game_has_ended = false;
        player current_player = player_one;
        while (!game_has_ended) {
            player_turn(current_player, chess_board);

            std::cout << std::endl << chess_board << std::endl;

            swap_player(current_player, player_one, player_two);
        }
    }
    
    return 0;
}