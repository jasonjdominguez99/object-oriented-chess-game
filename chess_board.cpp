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


#include <sstream>
#include "chess_board.hpp"
#include "chess_pieces.hpp"


using namespace brd;
using namespace pcs;


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

    chess_piece* board::operator[](int idx) const { 
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

        if (col_letter == 'A') return row_number*8 + 0;
        if (col_letter == 'B') return row_number*8 + 1;
        if (col_letter == 'C') return row_number*8 + 2;
        if (col_letter == 'D') return row_number*8 + 3;
        if (col_letter == 'E') return row_number*8 + 4;
        if (col_letter == 'F') return row_number*8 + 5;
        if (col_letter == 'G') return row_number*8 + 6;
        if (col_letter == 'H') return row_number*8 + 7;
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
        if (column_number == 1) position = "A";
        if (column_number == 2) position = "B";
        if (column_number == 3) position = "C";
        if (column_number == 4) position = "D";
        if (column_number == 5) position = "E";
        if (column_number == 6) position = "F";
        if (column_number == 7) position = "G";
        if (column_number == 8) position = "H";
        /*
        else {
            std::cerr << "Invalid board position.";
            exit(-1);
        }
        */

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
    chess_board[0] = new rook(white, 0);
    chess_board[1] = new knight(white, 1);
    chess_board[2] = new bishop(white, 2);
    chess_board[3] = new queen(white, 3);
    chess_board[4] = new king(white, 4);
    chess_board[5] = new bishop(white, 5);
    chess_board[6] = new knight(white, 6);
    chess_board[7] = new rook(white, 7);

    for (size_t i{1*8} ; i < 2*8 ; i++) {
        chess_board[i] = new pawn(white, i);
    }

    // Initialize unoccupied chess board squares
    // rows 3,4,5,6 (array elements 16-48)
    for (size_t i{2*8} ; i < 6*8 ; i++) {
        chess_board[i] = nullptr; 
    }

    // Initialize black pieces on chess board
    for (size_t i{6*8} ; i < 7*8 ; i++) {
        chess_board[i] = new pawn(black, i);
    }
        
    chess_board[7*8] = new rook(black, 7*8);
    chess_board[7*8 + 1] = new knight(black, 7*8 + 1);
    chess_board[7*8 + 2] = new bishop(black, 7*8 + 2);
    chess_board[7*8 + 3] = new queen(black, 7*8 + 3);
    chess_board[7*8 + 4] = new king(black, 7*8 + 4);
    chess_board[7*8 + 5] = new bishop(black, 7*8 + 5);
    chess_board[7*8 + 6] = new knight(black, 7*8 + 6);
    chess_board[7*8 + 7] = new rook(black, 7*8 + 7);
}

void board::move_piece(int initial_position, int final_position) {
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