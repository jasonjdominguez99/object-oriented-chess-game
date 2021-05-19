// OOPCpp_Final_Assignment_Chess/chess_board.cpp
//
// Source file for chess_board class
// function definitions
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 
// 24/04/2021 
// - Labelling of board (letters and numbers) corrected
// - Improved display of board with new letters and number
//   and flipped to be 1 at bottom 8 at top
// 25/04/2021
// - Changed raw pointers to smart shared_ptr pointer
// - Started using vector for chess board rather than array
// - Implemented using move semantics when moving chess
//   pieces
// 26/04/2021
// - Simplified move_piece method by removing uneccessary if-else
// 02/05/2021
// - Changed move_piece method to be compatible with en passant and 
//   castling moves
// 03/05/2021
// - Changed board positions to have lower case letters
// 06/05/2021
// - Removed using namespace to adhere to house style
// 18/05/2021
// - Implemented deep copying of chess board
// 19/05/2021
// - Changed all chess piece related shared pointers to 
//   unique, using move semantics for passing to functions


#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include "chess_board.hpp"
#include "chess_pieces.hpp"


// Class and function definitions
namespace brd {
    std::ostream & operator<<(std::ostream &output, const board &b) {
        output << std::endl << "     a   b   c   d   e   f   g   h" << std::endl;
        output << "  -----------------------------------" << std::endl;
        for (int i{7} ; i >= 0 ; i--) {
            output << i + 1 << " ||";
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
                        output << " " << b.chess_board[i*8 + j] << " || " << i + 1 << std::endl;
                    } else {
                        output << "   || " << i + 1 << std::endl;
                    }
                }
            }
            output << "  -----------------------------------" << std::endl;
        }
        output << "     a   b   c   d   e   f   g   h" << std::endl;

        return output;
    }

    std::unique_ptr<pcs::chess_piece> board::operator[](int idx) const { 
        // Check for out of bound index
        if (idx > 63 && idx >= 0) {
            std::cerr << "Board index out of range, exiting.";
            exit(-1); 
        }
        if (!chess_board[idx]) {
            return std::move(std::unique_ptr<pcs::chess_piece>{nullptr});
        } else {
            return std::move(chess_board[idx]->clone());
        }
    }

    int board_position_to_index(std::string position) {
        char col_letter = position[0];

        std::stringstream row_input;
        row_input << position[1];
        int row_number;
        row_input >> row_number;
        row_number -= 1;

        int position_index{};
        if (col_letter == 'a') { position_index = row_number*8 + 0; }
        else if (col_letter == 'b') { position_index = row_number*8 + 1; }
        else if (col_letter == 'c') { position_index = row_number*8 + 2; }
        else if (col_letter == 'd') { position_index = row_number*8 + 3; }
        else if (col_letter == 'e') { position_index = row_number*8 + 4; }
        else if (col_letter == 'f') { position_index = row_number*8 + 5; }
        else if (col_letter == 'g') { position_index = row_number*8 + 6; }
        else if (col_letter == 'h') { position_index = row_number*8 + 7; }
        else {
            return -1;
        }
        /*
        else {
            std::cerr << "Invalid board position.";
            exit(-1);
        }
        */

        return position_index;
    }

    std::string board_index_to_position(int position_index) {
        int row_number = position_index/8 + 1;
        int column_number = position_index%8 + 1;

        std::string position;
        if (column_number == 1) { position = "a"; }
        else if (column_number == 2) { position = "b"; }
        else if (column_number == 3) { position = "c"; }
        else if (column_number == 4) { position = "d"; }
        else if (column_number == 5) { position = "e"; }
        else if (column_number == 6) { position = "f"; }
        else if (column_number == 7) { position = "g"; }
        else if (column_number == 8) { position = "h"; }
        else {
            std::cerr << "Invalid board position.";
            exit(-1);
        }

        std::stringstream position_row;
        position_row << row_number;
        std::string row_label;
        position_row >> row_label;

        position += row_label;

        return position;
    }
}

brd::board::board() {
    // Initialize white pieces on chess board
    // starting from bottom left to top right
    chess_board.push_back(std::make_unique<pcs::rook>(pcs::rook(pcs::white, 0)));
    chess_board.push_back(std::make_unique<pcs::knight>(pcs::knight(pcs::white, 1)));
    chess_board.push_back(std::make_unique<pcs::bishop>(pcs::bishop(pcs::white, 2)));
    chess_board.push_back(std::make_unique<pcs::queen>(pcs::queen(pcs::white, 3)));
    chess_board.push_back(std::make_unique<pcs::king>(pcs::king(pcs::white, 4)));
    chess_board.push_back(std::make_unique<pcs::bishop>(pcs::bishop(pcs::white, 5)));
    chess_board.push_back(std::make_unique<pcs::knight>(pcs::knight(pcs::white, 6)));
    chess_board.push_back(std::make_unique<pcs::rook>(pcs::rook(pcs::white, 7)));

    for (size_t i{1*8} ; i < 2*8 ; i++) {
        chess_board.push_back(std::make_unique<pcs::pawn>(pcs::pawn(pcs::white, i)));
    }

    // Initialize unoccupied chess board squares
    // rows 3,4,5,6 (array elements 16-48)
    for (size_t i{2*8} ; i < 6*8 ; i++) {
        chess_board.push_back(std::unique_ptr<pcs::chess_piece>{nullptr}); 
    }

    // Initialize black pieces on chess board
    for (size_t i{6*8} ; i < 7*8 ; i++) {
        chess_board.push_back(std::make_unique<pcs::pawn>(pcs::pawn(pcs::black, i)));
    }
        
    chess_board.push_back(std::make_unique<pcs::rook>(pcs::rook(pcs::black, 7*8)));
    chess_board.push_back(std::make_unique<pcs::knight>(pcs::knight(pcs::black, 7*8 + 1)));
    chess_board.push_back(std::make_unique<pcs::bishop>(pcs::bishop(pcs::black, 7*8 + 2)));
    chess_board.push_back(std::make_unique<pcs::queen>(pcs::queen(pcs::black, 7*8 + 3)));
    chess_board.push_back(std::make_unique<pcs::king>(pcs::king(pcs::black, 7*8 + 4)));
    chess_board.push_back(std::make_unique<pcs::bishop>(pcs::bishop(pcs::black, 7*8 + 5)));
    chess_board.push_back(std::make_unique<pcs::knight>(pcs::knight(pcs::black, 7*8 + 6)));
    chess_board.push_back(std::make_unique<pcs::rook>(pcs::rook(pcs::black, 7*8 + 7)));
}

brd::board::board(const brd::board &board_to_copy) {
    // Perform deep copy of all chess piece pointers in chess board to copies 
    // chess board vector 
    for (size_t i{0} ; i < 8*8 ; i++) {
        if (!board_to_copy.chess_board[i]) {
            chess_board.push_back(std::unique_ptr<pcs::chess_piece>{nullptr});
        } else {
            chess_board.push_back(board_to_copy.chess_board[i]->clone());
        }
    }
}

std::vector<std::unique_ptr<pcs::chess_piece>> brd::board::get_board() const {
    std::vector<std::unique_ptr<pcs::chess_piece>> copy_of_board{};
    for (size_t i{0} ; i < 8*8 ; i++) {
        if (!chess_board[i]) {
            copy_of_board.push_back(std::unique_ptr<pcs::chess_piece>{nullptr});
        } else {
            copy_of_board.push_back(chess_board[i]->clone());
        }
    }
    return std::move(copy_of_board);
}

void brd::board::move_piece(int initial_position, int final_position, move_type move) {
    if (move == standard) {
        chess_board[final_position] = std::move(chess_board[initial_position]);
    } else if (move == en_passant) {
        // Piece which was captured en passant but not landed on, must be removed from board
        // so first move piece to be captured back, then capture as normal
        if (final_position == initial_position + 8 + 1 || final_position == initial_position + 8 - 1) {
            chess_board[final_position] = std::move(chess_board[final_position - 8]);
        } else if (final_position == initial_position - 8 + 1 || final_position == initial_position - 8 - 1) {
            chess_board[final_position] = std::move(chess_board[final_position + 8]);
        } else {
            std::cerr << "Invalid en passant capture, exiting.";
            exit(-1);
        }
        chess_board[final_position] = std::move(chess_board[initial_position]);
        
    } else if (move == castling) {
        if (chess_board[initial_position]->get_symbol() == 'K' && chess_board[initial_position + 3]->get_symbol() == 'R' &&
            !chess_board[final_position] && !chess_board[final_position - 1]) {

            chess_board[final_position - 1] = std::move(chess_board[initial_position + 3]); // Move rook
        } else if (chess_board[initial_position]->get_symbol() == 'K' && chess_board[initial_position - 4]->get_symbol() == 'R'&&
            !chess_board[final_position] && !chess_board[final_position + 1]) {

            chess_board[final_position + 1] = std::move(chess_board[initial_position - 4]); // Move rook
        } else {
            std::cerr << "Invalid castling move, exiting.";
            exit(-1);
        }
        chess_board[final_position] = std::move(chess_board[initial_position]); // Move king as normal

    } else {
        std::cerr << "Invalid move type, exiting.";
        exit(-1); 
    }
    
}