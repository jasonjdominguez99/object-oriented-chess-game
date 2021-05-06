// OOPCpp_Final_Assignment_Chess/chess_pieces.cpp
//
// Source file for chess_piece class
// and derived classes function definitions
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 
// 24/04/2021
// - Fixed error with checking for diagonal moves with king
// - Added functionality to check if king will move into
//   checkmate position (i.e. make sure it doesn't move somewhere
//   where it can be captured)
// - Added function to get all possible moves for 
//   a given chess piece color
// 25/04/2021
// - Edited get valid moves functions to take a pointer to a chess piece pointer
//   rather than chess board, to reduce interclass dependence
// - Changed raw pointers to smart shared_ptr pointer
// - Started using vector for chess board rather than array
// 26/04/2021
// - Fix bug with king being allowed to capture a piece which would
//   lead to it being captured
// 30/04/2021
// - Restructured entire code to include chess_game class
//   still capable of the same functionality, but now
//   able to start on chess engine
// - Fixed bug with knight giving incorrect possible moves which
//   when out of range of the board
// - Possible move functions now don't consider whether king is moved
//   into check, that it done in chess_game class
// 01/05/2021
// - Fixed bug with queen being able to jump over pieces
// - Fixed bug with pawn being able to jump over piece
//   if taking two steps on its first move
// - Used ANSI to output chess letter symbols in gray for black pieces
//   and bold for both colored pieces
// 02/05/2021
// - Added functionality to give castling as a possible move for king
// - Added en passant functionality to pawn
// - Fixed bug with diagonal moves jumping over opposition pieces
// 06/05/2021
// - Removed using namespace to adhere to house style


#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <iterator>
#include "chess_pieces.hpp"


// Class and function definitions
namespace pcs {
    std::ostream & operator<<(std::ostream &output, std::shared_ptr<chess_piece> const& piece) {
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

    std::vector<int> get_all_possible_moves(color piece_color, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
        std::vector<int> all_possible_moves;
        for (int i{} ; i < 8*8 ; i++) {
            if (chess_board[i] && chess_board[i]->get_piece_color() == piece_color) {
                std::vector<int> piece_possible_moves;
                piece_possible_moves = chess_board[i]->get_valid_moves(i, chess_board);
            
                all_possible_moves.insert(std::end(all_possible_moves), 
                                          std::begin(piece_possible_moves),
                                          std::end(piece_possible_moves));
            }
        }

        return all_possible_moves;
    }
}

// Define functions to get valid move for chess pieces
std::vector<int> pcs::pawn::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> pcs::rook::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> pcs::knight::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};
        
    std::vector<int> valid_new_positions;

    // Knights can move in L-shapes (e.g. 2 forward 1 left)

    // Check possible L-shape moves
    // Check 2 forward 1 right and 2 forward 1 left
    if (start_position_row + 2 < 8) {
        if (start_position_col + 1 < 8) {
            if (!chess_board[start_position + (2*8 + 1)] || chess_board[start_position + (2*8 + 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (2*8 + 1)); // move up 2 and right 1
            }
        }
        if (start_position_col - 1 >= 0) {
            if (!chess_board[start_position + (2*8 - 1)] || chess_board[start_position + (2*8 - 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (2*8 - 1)); // move up 2 and left 1
            }
        }
    }
    // Check 1 forward 2 right and 1 forward 2 left
    if (start_position_row + 1 < 8) {
        if (start_position_col + 2 < 8) {
            if (!chess_board[start_position + (8 + 2)] || chess_board[start_position + (8 + 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (8 + 2)); // move up 1 and right 2
            }
        }
        if (start_position_col - 2 >= 0) {
            if (!chess_board[start_position + (8 - 2)] || chess_board[start_position + (8 - 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (8 - 2)); // move up 1 and left 2
            }
        }
    }
    // Check 1 backward 2 right and 1 backward 2 left
    if (start_position_row - 1 >= 0) {
        if (start_position_col + 2 < 8) {
            if (!chess_board[start_position + (-8 + 2)] || chess_board[start_position + (-8 + 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-8 + 2)); // move down 1 and right 2
            }
        }
        if (start_position_col - 2 >= 0) {
            if (!chess_board[start_position + (-8 - 2)] || chess_board[start_position + (-8 - 2)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-8 - 2)); // move down 1 and left 2
            }
        }
    }
    // Check 2 backward 1 right and 2 backward 1 left
    if (start_position_row - 2 >= 0) {
        if (start_position_col + 1 < 8) {
            if (!chess_board[start_position + (-2*8 + 1)] || chess_board[start_position + (-2*8 + 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-2*8 + 1)); // move down 2 and right 1
            }
        }
        if (start_position_col - 1 >= 0) {
            if (!chess_board[start_position + (-2*8 - 1)] || chess_board[start_position + (-2*8 - 1)]->get_piece_color() != this->get_piece_color()) {
                valid_new_positions.push_back(start_position + (-2*8 - 1)); // move down 2 and left 1
            }
        }
    }

    return valid_new_positions;
}

std::vector<int> pcs::bishop::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
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
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
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
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
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

    return valid_new_positions;
}

std::vector<int> pcs::queen::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
    int start_position_row{start_position/8};
    int start_position_col{start_position%8};

    std::vector<int> valid_new_positions;

    // Queens can move diagonally, vertically or horizontally any number
    // of spaces, but cannot jump over any other chess pieces

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
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
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
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
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
                if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() == this->get_piece_color()) {
                    is_blocked = true;
                    break;
                } else {
                    valid_new_positions.push_back(8*j + i);
                    // If space contains opposite color piece, it cannot be jumped over
                    if (chess_board[8*j + i] && chess_board[8*j + i]->get_piece_color() != this->get_piece_color()) {
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


    return valid_new_positions;
}

std::vector<int> pcs::king::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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
        if (chess_board[start_position + 3] &&
            chess_board[start_position + 3]->get_symbol() == 'R' &&
            !chess_board[start_position + 3]->get_has_moved()) {

            if (!chess_board[start_position + 1] &&
                !chess_board[start_position + 2]) {

                valid_new_positions.push_back(start_position + 2);
            }
        }
    }
    // Queenside castling (castling with rook furthest from king)
    if (!this->has_moved) {
        if (chess_board[start_position - 4] &&
            chess_board[start_position - 4]->get_symbol() == 'R' &&
            !chess_board[start_position - 4]->get_has_moved()) {

            if (!chess_board[start_position - 1] &&
                !chess_board[start_position - 2] &&
                !chess_board[start_position - 3]) {

                valid_new_positions.push_back(start_position - 2);
            }
        }
    }

    
    return valid_new_positions;
}