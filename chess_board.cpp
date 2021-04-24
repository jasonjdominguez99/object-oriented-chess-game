// OOPCpp_Final_Assignment_Chess/chess_board.cpp
//
// Source file for chess_board class
// function definitions
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 23/04/2021


#include <sstream>
#include "chess_board.hpp"
#include "chess_pieces.hpp"


using namespace brd;
using namespace pcs;


namespace brd {
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

    chess_piece* board::operator[](int idx) const { 
        // Check for out of bound index
        if (idx > 63 && idx >= 0) {
            std::cerr << "Board index out of range, exiting.";
            exit(-1); 
        }
        return chess_board[idx];
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
}

board::board() {
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