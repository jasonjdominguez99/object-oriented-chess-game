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


#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>
#include "chess_pieces.hpp"


using namespace pcs;


namespace pcs {
    std::ostream & operator<<(std::ostream &output, std::shared_ptr<chess_piece> const& piece) {
        output << piece->piece_symbol;
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
                if (chess_board[i]->get_symbol() == 'K') {
                    // In the case of a king, assume it can do all its possible moves, to avoid
                    // infinite loop trying to get king's possible moves
                    if (i/8 < 7) { piece_possible_moves.push_back(i + 8); }
                    if (i/8 > 0) { piece_possible_moves.push_back(i - 8); }
                    if (i%8 < 7) { piece_possible_moves.push_back(i + 1); }
                    if (i%8 > 0) { piece_possible_moves.push_back(i - 1); }
                } else {
                    piece_possible_moves = chess_board[i]->get_valid_moves(i, chess_board);
                }
            
                all_possible_moves.insert(std::end(all_possible_moves), 
                                          std::begin(piece_possible_moves),
                                          std::end(piece_possible_moves));
            }
        }

        return all_possible_moves;
    }
}

// Define functions to get valid move for chess pieces
std::vector<int> pawn::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> rook::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> knight::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> bishop::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> queen::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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

std::vector<int> king::get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board) {
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
    
    // Check that none of the 'allowed' moves puts the king in a position to get taken
    std::vector<int> all_opposition_possible_moves;
    all_opposition_possible_moves = get_all_possible_moves(opposite_color(this->get_piece_color()), chess_board);
    // Remove any of these opposition moves from the possible moves
    auto move_found = [&all_opposition_possible_moves](const int& position) -> bool {
        return std::find(all_opposition_possible_moves.begin(),
                         all_opposition_possible_moves.end(), 
                         position) != all_opposition_possible_moves.end();
    };
    valid_new_positions.erase(std::remove_if(valid_new_positions.begin(),
                                             valid_new_positions.end(),
                                             move_found), valid_new_positions.end());

    
    return valid_new_positions;
}