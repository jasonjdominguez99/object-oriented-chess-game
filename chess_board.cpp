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


#include <sstream>
#include <memory>
#include "chess_board.hpp"
#include "chess_pieces.hpp"


using namespace brd;


namespace brd {
    std::ostream & operator<<(std::ostream &output, const board &b) {
        output << std::endl << "     A   B   C   D   E   F   G   H" << std::endl;
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
        output << "     A   B   C   D   E   F   G   H" << std::endl;

        return output;
    }

    std::shared_ptr<pcs::chess_piece> board::operator[](int idx) const { 
        // Check for out of bound index
        if (idx > 63 && idx >= 0) {
            std::cerr << "Board index out of range, exiting.";
            exit(-1); 
        }
        return chess_board[idx];
    }

    int board_position_to_index(std::string position) {
        char col_letter = position[0];

        std::stringstream row_input;
        row_input << position[1];
        int row_number;
        row_input >> row_number;
        row_number -= 1;

        int position_index{};
        if (col_letter == 'A') { position_index = row_number*8 + 0; }
        else if (col_letter == 'B') { position_index = row_number*8 + 1; }
        else if (col_letter == 'C') { position_index = row_number*8 + 2; }
        else if (col_letter == 'D') { position_index = row_number*8 + 3; }
        else if (col_letter == 'E') { position_index = row_number*8 + 4; }
        else if (col_letter == 'F') { position_index = row_number*8 + 5; }
        else if (col_letter == 'G') { position_index = row_number*8 + 6; }
        else if (col_letter == 'H') { position_index = row_number*8 + 7; }
        else {
            std::cerr << "Invalid board position.";
            exit(-1);
        }

        return position_index;
    }

    std::string board_index_to_position(int position_index) {
        int row_number = position_index/8 + 1;
        int column_number = position_index%8 + 1;

        std::string position;
        if (column_number == 1) { position = "A"; }
        else if (column_number == 2) { position = "B"; }
        else if (column_number == 3) { position = "C"; }
        else if (column_number == 4) { position = "D"; }
        else if (column_number == 5) { position = "E"; }
        else if (column_number == 6) { position = "F"; }
        else if (column_number == 7) { position = "G"; }
        else if (column_number == 8) { position = "H"; }
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

board::board() {
    // Initialize white pieces on chess board
    // starting from bottom left to top right
    chess_board.push_back(std::make_shared<pcs::rook>(pcs::rook(pcs::white, 0)));
    chess_board.push_back(std::make_shared<pcs::knight>(pcs::knight(pcs::white, 1)));
    chess_board.push_back(std::make_shared<pcs::bishop>(pcs::bishop(pcs::white, 2)));
    chess_board.push_back(std::make_shared<pcs::queen>(pcs::queen(pcs::white, 3)));
    chess_board.push_back(std::make_shared<pcs::king>(pcs::king(pcs::white, 4)));
    chess_board.push_back(std::make_shared<pcs::bishop>(pcs::bishop(pcs::white, 5)));
    chess_board.push_back(std::make_shared<pcs::knight>(pcs::knight(pcs::white, 6)));
    chess_board.push_back(std::make_shared<pcs::rook>(pcs::rook(pcs::white, 7)));

    for (size_t i{1*8} ; i < 2*8 ; i++) {
        chess_board.push_back(std::make_shared<pcs::pawn>(pcs::pawn(pcs::white, i)));
    }

    // Initialize unoccupied chess board squares
    // rows 3,4,5,6 (array elements 16-48)
    for (size_t i{2*8} ; i < 6*8 ; i++) {
        chess_board.push_back(std::shared_ptr<pcs::chess_piece>{nullptr}); 
    }

    // Initialize black pieces on chess board
    for (size_t i{6*8} ; i < 7*8 ; i++) {
        chess_board.push_back(std::make_shared<pcs::pawn>(pcs::pawn(pcs::black, i)));
    }
        
    chess_board.push_back(std::make_shared<pcs::rook>(pcs::rook(pcs::black, 7*8)));
    chess_board.push_back(std::make_shared<pcs::knight>(pcs::knight(pcs::black, 7*8 + 1)));
    chess_board.push_back(std::make_shared<pcs::bishop>(pcs::bishop(pcs::black, 7*8 + 2)));
    chess_board.push_back(std::make_shared<pcs::queen>(pcs::queen(pcs::black, 7*8 + 3)));
    chess_board.push_back(std::make_shared<pcs::king>(pcs::king(pcs::black, 7*8 + 4)));
    chess_board.push_back(std::make_shared<pcs::bishop>(pcs::bishop(pcs::black, 7*8 + 5)));
    chess_board.push_back(std::make_shared<pcs::knight>(pcs::knight(pcs::black, 7*8 + 6)));
    chess_board.push_back(std::make_shared<pcs::rook>(pcs::rook(pcs::black, 7*8 + 7)));
}

void board::move_piece(int initial_position, int final_position) {
    chess_board[final_position] = std::move(chess_board[initial_position]);
    /*
    if (!chess_board[final_position]) {
        // Final position on chess board is empty
        chess_board[final_position] = std::move(chess_board[initial_position]);
    } else {
        // Final position has a chess piece already there
        // so remove it from the board
        //removed_chess_pieces.push_back(chess_board[final_position]);
        chess_board[final_position] = std::move(chess_board[initial_position]);
    }
    */
}