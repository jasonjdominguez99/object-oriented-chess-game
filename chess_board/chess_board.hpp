// OOPCpp_Final_Assignment_Chess/chess_board.h
//
// Header file for chess_board class
// 
// Author: Jason Dominguez


#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <iostream>
#include <vector>
#include <memory>


// Forward declarations
namespace pcs {
    class chess_piece;
}


// Class and function definitions
namespace brd {
    int board_position_to_index(std::string position);
    std::string board_index_to_position(int position_index);
    
    enum move_type {standard, en_passant, castling};

    std::vector<std::unique_ptr<pcs::chess_piece>> initialize_board(std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

    class board
    {
        // Friend function for overloading the output operator
        friend std::ostream & operator<<(std::ostream &output, const board &b);

    private:
        std::vector<std::unique_ptr<pcs::chess_piece>> chess_board;

    public:
        board();
        board(const board &board_to_copy);
        board(std::vector<std::unique_ptr<pcs::chess_piece>> &board_to_copy);
        ~board() {};

        void move_piece(int initial_position, int final_position, move_type move);
        void move_piece(int initial_position, int final_position);
        std::unique_ptr<pcs::chess_piece> operator[](int idx) const;
        std::unique_ptr<pcs::chess_piece> & operator[](int idx);
        std::vector<std::unique_ptr<pcs::chess_piece>> get_board() const;
        void reset();
    };  
}

#endif