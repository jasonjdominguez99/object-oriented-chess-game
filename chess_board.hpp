// OOPCpp_Final_Assignment_Chess/chess_board.h
//
// Header file for chess_board class
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 
// 24/04/2021
// - Added function to return the chess board


#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <iostream>
#include <vector>
#include <memory>


namespace pcs {
    class chess_piece;
}


namespace brd {
    int board_position_to_index(std::string position);
    std::string board_index_to_position(int position_index);
    
    class board
    {
        // Friend function for overloading the output operator
        friend std::ostream & operator<<(std::ostream &output, const board &b);

    private:
        //std::unique_ptr<pcs::chess_piece*[]> chess_board{new pcs::chess_piece*[8*8]}; // Try smart pointers later
        pcs::chess_piece **chess_board = new pcs::chess_piece*[8*8]; //8x8 chess board
        std::vector<pcs::chess_piece*> removed_chess_pieces;

    public:
        board();
        ~board() {
            for (int i{0} ; i < 8 ; i++) {
                for (int j{0} ; j < 8 ; j++) {
                    delete chess_board[i*8 + j];
                }
            }
            delete[] chess_board;
            std::cout << "Destructor called." << std::endl;
        }


        void move_piece(int initial_position, int final_position);
        pcs::chess_piece* operator[](int idx) const;
        pcs::chess_piece** get_board() {
            return chess_board;
        }
    };  
}

#endif