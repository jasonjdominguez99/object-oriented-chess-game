// OOPCpp_Final_Assignment_Chess/chess_pieces.cpp
//
// Source file for chess_piece class
// and derived classes function definitions
// 
// Author: Jason Dominguez


#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <tuple>
#include "chess_pieces.hpp"


// Class and function definitions
namespace pcs {
    std::ostream & operator<<(std::ostream &output, std::unique_ptr<chess_piece> const& piece) {
        // Diplay chess piece symbol using ANSI to make symbols bold and 
        // for blakc pieces gray
        std::stringstream symbol_output_in_ansi;
        if (piece->get_piece_color() == white) {
            symbol_output_in_ansi << "\u001b[1m" << piece->piece_symbol << "\u001b[0m";
        } else {
            symbol_output_in_ansi << "\u001b[1m\u001b[30m" << piece->piece_symbol << "\u001b[0m";
        }
        output << symbol_output_in_ansi.str();
        return output;
    }

    std::string color_to_string(color chess_piece_color) {
        switch(chess_piece_color) {
            case white:
                return "white";
            case black:
                return "black";
            default:
                throw std::invalid_argument("Invalid chess piece color");
        }
    }

    color string_to_color(std::string chess_piece_color) {
        if (chess_piece_color == "white" || chess_piece_color == "White") {
            return white;
        } else if (chess_piece_color == "black" || chess_piece_color == "Black") {
            return black;
        } else {
            throw std::invalid_argument("Invalid chess piece color");
        }
    }

    color opposite_color(color chess_piece_color) {
        if (chess_piece_color == white) {
            return black;
        } else if (chess_piece_color == black) {
            return white;
        } else {
            throw std::invalid_argument("Invalid chess piece color");
        }
    }


    std::tuple<std::vector<int>, 
               std::vector<std::vector<int>>,
               std::vector<int>> get_all_possible_moves(color piece_color, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
        
        // Want to return possible positions of piece to move, possible moves for each and all possible moves
        std::vector<int> possible_start_positions{};
        std::vector<std::vector<int>> possible_moves_per_piece{};
        std::vector<int> all_possible_moves{};
        for (int i{} ; i < 8*8 ; i++) {
            if (chess_board[i] && chess_board[i]->get_piece_color() == piece_color) {
                std::vector<int> piece_possible_moves;
                // Create temporary copy of chess_board
                std::vector<std::unique_ptr<pcs::chess_piece>> copy_of_board{};
                for (size_t i{0} ; i < 8*8 ; i++) {
                    if (!chess_board[i]) {
                        copy_of_board.push_back(std::unique_ptr<pcs::chess_piece>{nullptr});
                    } else {
                        copy_of_board.push_back(chess_board[i]->clone());
                    }
                }
                piece_possible_moves = chess_board[i]->get_valid_moves(i, std::move(copy_of_board));

                possible_start_positions.push_back(i);
                possible_moves_per_piece.push_back(piece_possible_moves);
                all_possible_moves.insert(std::end(all_possible_moves), std::begin(piece_possible_moves), std::end(piece_possible_moves));
            }
        }
        return std::make_tuple(possible_start_positions, possible_moves_per_piece, all_possible_moves);
    }


    void look_for_horizontal_and_vertical_moves(color piece_color,
                                                std::vector<int>& valid_new_positions, 
                                                int start_position_col, int start_position_row, 
                                                std::vector<std::unique_ptr<pcs::chess_piece>>const& chess_board) {
        
        // Find valid horizontal moves
        for (int i{start_position_col + 1} ; i < 8 ; i++) {
            if (chess_board[8*start_position_row + i] && 
                chess_board[8*start_position_row + i]->get_piece_color() == piece_color) {
                break;
            } else {
                valid_new_positions.push_back(8*start_position_row + i); // Move right
                // If space contains opposite color piece, it cannot be jumped over
                if (chess_board[8*start_position_row + i] && 
                    chess_board[8*start_position_row + i]->get_piece_color() != piece_color) {
                    break;
                }
            }
        }
        for (int i{start_position_col - 1} ; i >= 0 ; i--) {
            if (chess_board[8*start_position_row + i] && 
                chess_board[8*start_position_row + i]->get_piece_color() == piece_color) {
                break;
            } else {
                valid_new_positions.push_back(8*start_position_row + i); // Move left
                // If space contains opposite color piece, it cannot be jumped over
                if (chess_board[8*start_position_row + i] && 
                    chess_board[8*start_position_row + i]->get_piece_color() != piece_color) {
                    break;
                }
            }
        }
        // Find valid vertical moves
        for (int i{start_position_row + 1} ; i < 8 ; i++) {
            if (chess_board[8*i + start_position_col] && 
                chess_board[8*i + start_position_col]->get_piece_color() == piece_color) {
                break;
            } else {
                valid_new_positions.push_back(8*i + start_position_col); // Move forward
                // If space contains opposite color piece, it cannot be jumped over
                if (chess_board[8*i + start_position_col] && 
                    chess_board[8*i + start_position_col]->get_piece_color() != piece_color) {
                    break;
                }
            }
        }
        for (int i{start_position_row - 1} ; i >= 0 ; i--) {
            if (chess_board[8*i + start_position_col] && 
                chess_board[8*i + start_position_col]->get_piece_color() == piece_color) {
                break;
            } else {
                valid_new_positions.push_back(8*i + start_position_col); // Move backward
                // If space contains opposite color piece, it cannot be jumped over
                if (chess_board[8*i + start_position_col] && 
                    chess_board[8*i + start_position_col]->get_piece_color() != piece_color) {
                    break;
                }
            }
        }
    }


    void look_for_diagonal_moves(color piece_color,
                                 std::vector<int>& valid_new_positions, 
                                 int start_position_col, int start_position_row, 
                                 std::vector<std::unique_ptr<pcs::chess_piece>>const& chess_board) {

        // hi
        // Find valid diagonal moves
        // Moving up and to the right
        bool is_blocked = false;
        for (int i{start_position_col + 1} ; i < 8 ; i++) {
            for (int j{start_position_row + 1} ; j < 8 ; j++) {
                if ( (start_position_col - i) == (start_position_row - j) ) {
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == piece_color) {
                        is_blocked = true;
                        break;
                    } else {
                        valid_new_positions.push_back(8*j + i);
                        // If space contains opposite color piece, it cannot be jumped over
                        if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != piece_color) {
                            is_blocked = true;
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
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == piece_color) {
                        is_blocked = true;
                        break;
                    } else {
                        valid_new_positions.push_back(8*j + i);
                        // If space contains opposite color piece, it cannot be jumped over
                        if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != piece_color) {
                            is_blocked = true;
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
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == piece_color) {
                        is_blocked = true;
                        break;
                    } else {
                        valid_new_positions.push_back(8*j + i);
                        // If space contains opposite color piece, it cannot be jumped over
                        if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != piece_color) {
                            is_blocked = true;
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
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == piece_color) {
                        is_blocked = true;
                        break;
                    } else {
                        valid_new_positions.push_back(8*j + i);
                        // If space contains opposite color piece, it cannot be jumped over
                        if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != piece_color) {
                            is_blocked = true;
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
    } 
}


// Define functions to get valid move for chess pieces
std::vector<int> pcs::pawn::get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
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
            if (!chess_board[start_position + 2*8*position_increment] && 
                !chess_board[start_position + 8*position_increment]) {

                valid_new_positions.push_back(start_position + 2*8*position_increment); // moving forward by 2
            }
        }
    }
    // Check if diagonal moves are possible (if attacking opposite color piece or if en passant is possible)
    if (start_position_row + position_increment < 8 && start_position_col + position_increment < 8) {
        if ((chess_board[start_position + (8*position_increment + position_increment)] && 
             chess_board[start_position + (8*position_increment + position_increment)]->get_piece_color() != this->get_piece_color()) ||
            (chess_board[start_position + position_increment] && 
             chess_board[start_position + position_increment]->get_piece_color() != this->get_piece_color() &&
             chess_board[start_position + position_increment]->get_symbol() == 'p' &&
             chess_board[start_position + position_increment]->is_en_passant_possible() == true)) {
            
            valid_new_positions.push_back(start_position + (8*position_increment + position_increment)); // moving forward and right by 1
        }
    }
    if (start_position_row + position_increment < 8 && start_position_col - position_increment >= 0) {
        if ((chess_board[start_position + (8*position_increment - position_increment)] && 
             chess_board[start_position + (8*position_increment - position_increment)]->get_piece_color() != this->get_piece_color()) ||
            (chess_board[start_position - position_increment] && 
             chess_board[start_position - position_increment]->get_piece_color() != this->get_piece_color() &&
             chess_board[start_position - position_increment]->get_symbol() == 'p' &&
             chess_board[start_position - position_increment]->is_en_passant_possible() == true)) {
            
            valid_new_positions.push_back(start_position + (8*position_increment - position_increment)); // moving forward and left by 1
        }
    }

    return valid_new_positions;
}


std::vector<int> pcs::rook::get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Rooks can move vertically or horizontally any number of spaces, but cannot
    // jump over any other chess pieces

    look_for_horizontal_and_vertical_moves(this->get_piece_color(),
                                           valid_new_positions, 
                                           start_position_col, start_position_row, 
                                           chess_board);

    return valid_new_positions;
}


std::vector<int> pcs::knight::get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};
        
    std::vector<int> valid_new_positions;

    // Knights can move in L-shapes (e.g. 2 forward 1 left)

    // Check possible L-shape moves
    // Check 2 forward 1 right and 2 forward 1 left
    if (start_position_row + 2 < 8) {
        if (start_position_col + 1 < 8) {
            if (!chess_board[start_position + (2*8 + 1)] || 
                chess_board[start_position + (2*8 + 1)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (2*8 + 1)); // move up 2 and right 1
            }
        }
        if (start_position_col - 1 >= 0) {
            if (!chess_board[start_position + (2*8 - 1)] || 
                chess_board[start_position + (2*8 - 1)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (2*8 - 1)); // move up 2 and left 1
            }
        }
    }
    // Check 1 forward 2 right and 1 forward 2 left
    if (start_position_row + 1 < 8) {
        if (start_position_col + 2 < 8) {
            if (!chess_board[start_position + (8 + 2)] || 
                chess_board[start_position + (8 + 2)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (8 + 2)); // move up 1 and right 2
            }
        }
        if (start_position_col - 2 >= 0) {
            if (!chess_board[start_position + (8 - 2)] || 
                chess_board[start_position + (8 - 2)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (8 - 2)); // move up 1 and left 2
            }
        }
    }
    // Check 1 backward 2 right and 1 backward 2 left
    if (start_position_row - 1 >= 0) {
        if (start_position_col + 2 < 8) {
            if (!chess_board[start_position + (-8 + 2)] || 
                chess_board[start_position + (-8 + 2)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (-8 + 2)); // move down 1 and right 2
            }
        }
        if (start_position_col - 2 >= 0) {
            if (!chess_board[start_position + (-8 - 2)] || 
                chess_board[start_position + (-8 - 2)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (-8 - 2)); // move down 1 and left 2
            }
        }
    }
    // Check 2 backward 1 right and 2 backward 1 left
    if (start_position_row - 2 >= 0) {
        if (start_position_col + 1 < 8) {
            if (!chess_board[start_position + (-2*8 + 1)] || 
                chess_board[start_position + (-2*8 + 1)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (-2*8 + 1)); // move down 2 and right 1
            }
        }
        if (start_position_col - 1 >= 0) {
            if (!chess_board[start_position + (-2*8 - 1)] || 
                chess_board[start_position + (-2*8 - 1)]->get_piece_color() != this->get_piece_color()) 
            {
                valid_new_positions.push_back(start_position + (-2*8 - 1)); // move down 2 and left 1
            }
        }
    }
    return valid_new_positions;
}


std::vector<int> pcs::bishop::get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Bishops can move diagonally any number of spaces, but cannot
    // jump over any other chess pieces

    look_for_diagonal_moves(this->get_piece_color(),
                            valid_new_positions, 
                            start_position_col, start_position_row, 
                            chess_board);

    return valid_new_positions;
}


std::vector<int> pcs::queen::get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Queens can move diagonally, vertically or horizontally any number
    // of spaces, but cannot jump over any other chess pieces

    look_for_horizontal_and_vertical_moves(this->get_piece_color(),
                                           valid_new_positions, 
                                           start_position_col, start_position_row, 
                                           chess_board);

    look_for_diagonal_moves(this->get_piece_color(),
                            valid_new_positions, 
                            start_position_col, start_position_row, 
                            chess_board);
    
    return valid_new_positions;
}

std::vector<int> pcs::king::get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board) {
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
        if (!chess_board[start_position + (8 + 1)] || chess_board[start_position + (8 + 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + (8 + 1)); // move forward and right by 1
        }
    }
    if (start_position_row + 1 < 8 && start_position_col - 1 >= 0) {
        if (!chess_board[start_position + (8 - 1)] || chess_board[start_position + (8 - 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position + (8 - 1)); // move forward and left by 1
        }
    }
    if (start_position_row - 1 >= 0 && start_position_col + 1 < 8) {
        if (!chess_board[start_position - (8 - 1)] || chess_board[start_position - (8 - 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position - (8 - 1)); // move backwards and right by 1
        }
    }
    if (start_position_row - 1 >= 0 && start_position_col - 1 >= 0) {
        if (!chess_board[start_position - (8 + 1)] || chess_board[start_position - (8 + 1)]->get_piece_color() != this->get_piece_color()) {
            valid_new_positions.push_back(start_position - (8 + 1)); // moving backwards and left by 1
        }
    }

    // Also check for the possibility of castling
    // Kingside castling (castling with closest rook to king)
    if (!this->has_moved) {
        if (chess_board[start_position + 3] && chess_board[start_position + 3]->get_symbol() == 'R' &&
            !chess_board[start_position + 3]->get_has_moved()) 
        {
            if (!chess_board[start_position + 1] && !chess_board[start_position + 2]) {
                valid_new_positions.push_back(start_position + 2);
            }
        }
    }
    // Queenside castling (castling with rook furthest from king)
    if (!this->has_moved) {
        if (chess_board[start_position - 4] && chess_board[start_position - 4]->get_symbol() == 'R' &&
            !chess_board[start_position - 4]->get_has_moved()) 
        {
            if (!chess_board[start_position - 1] && !chess_board[start_position - 2] && !chess_board[start_position - 3]) {
                valid_new_positions.push_back(start_position - 2);
            }
        }
    }
    return valid_new_positions;
}