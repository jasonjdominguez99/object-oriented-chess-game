// OOPCpp_Final_Assignment_Chess/chess_board.h
//
// Header file for chess_board class
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 
// 24/04/2021
// - Added function to return the chess board
// 25/04/2021
// - Changed raw pointers to smart shared_ptr pointer
// - Started using vector for chess board rather than array


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
        std::vector<std::shared_ptr<pcs::chess_piece>> chess_board;
        //std::vector<std::unique_ptr<pcs::chess_piece>> removed_chess_pieces;

    public:
        board();
        ~board() {};

        void move_piece(int initial_position, int final_position);
        std::shared_ptr<pcs::chess_piece> operator[](int idx) const;
        std::vector<std::shared_ptr<pcs::chess_piece>> get_board() {
            return chess_board;
        }
    };  
}

#endif