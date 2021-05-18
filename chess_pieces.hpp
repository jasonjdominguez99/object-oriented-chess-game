// OOPCpp_Final_Assignment_Chess/chess_pieces.h
//
// Header file for chess_piece class
// and derived classes 
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 
// 24/04/2021
// - Added function to get all possible moves for 
//   a given chess piece color
// 25/04/2021
// - Edited get valid moves functions to take a pointer to a chess piece pointer
//   rather than chess board, to reduce interclass dependence
// - Changed raw pointers to smart shared_ptr pointer
// 26/04/2021
// - Removed repeated has_moved attribute from derived chess_piece classes
//   which resulted in pawns being able to move two forward all the time
// 30/04/2021
// - Restructured entire code to include chess_game class
//   still capable of the same functionality, but now
//   able to start on chess engine
// 02/05/2021
// - Added private member variable to pawn class to say whether it can be
//   captured via en passant
// 18/05/2021
// - Created clone function for pieces to help with deep copying of 
//   chess board


#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H


#include <iostream>
#include <vector>
#include <memory>


// Class and function definitions
namespace pcs {
    enum color {white, black};
    std::string color_to_string(color chess_piece_color);
    color string_to_color(std::string chess_piece_color);
    color opposite_color(color chess_piece_color);

    class chess_piece
    {
        // Friend function for overloading the output operator
        friend std::ostream & operator<<(std::ostream &output, std::shared_ptr<chess_piece> const& piece);

    protected:
        color piece_color{};
        int piece_id{};
        char piece_symbol{};
        bool has_moved{};
        bool en_passant_is_possible{};

    public:
        // Constructors
        chess_piece() {
            piece_color = white;
            piece_id = 0; // id of 0 used to denoted chess poece not present on board
            piece_symbol = ' ';
            has_moved = false;
            en_passant_is_possible = false;
        }
        chess_piece(color white_or_black, int id, char symbol): 
            piece_color{white_or_black}, piece_id{id}, piece_symbol{symbol}, has_moved{false} {}
        // Destructor
        virtual ~chess_piece(){}

        // Member functions
        color get_piece_color() { return piece_color; }
        int get_id() { return piece_id; }
        char get_symbol() { return piece_symbol; }
        bool get_has_moved() { return has_moved; }
        void has_been_moved() { has_moved = true; }
        bool is_en_passant_possible() { return en_passant_is_possible; }
        void set_en_passant_possibility(bool en_passant_possibility) { en_passant_is_possible = en_passant_possibility; }
        virtual std::shared_ptr<chess_piece> clone() const = 0;

        chess_piece & operator=(chess_piece &chess_piece_to_copy) {
            if (&chess_piece_to_copy == this) return *this; // account for self-assignment
        
            piece_color = chess_piece_to_copy.piece_color;
            piece_id = chess_piece_to_copy.piece_id;

            return *this;
        }
        virtual std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board)=0;
    };

    std::vector<int> get_all_possible_moves(color piece_color, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

    class pawn : public chess_piece
    {
    public:
        // Constructors
        pawn() : chess_piece() {}
        pawn(color white_or_black, int id) : chess_piece(white_or_black, id, 'p') {}
        // Destructor
        virtual ~pawn(){}

        // Member functions
        std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

        std::shared_ptr<chess_piece> clone() const {
            return std::make_shared<pawn>(pcs::pawn(this->piece_color, this->piece_id));
        }
    };

    class rook : public chess_piece
    {  
    public:
        // Constructors
        rook() : chess_piece() {}
        rook(color white_or_black, int id) : chess_piece(white_or_black, id, 'R') {}
        // Destructor
        virtual ~rook(){}

        // Member functions
        std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

        std::shared_ptr<chess_piece> clone() const {
            return std::make_shared<rook>(pcs::rook(this->piece_color, this->piece_id));
        }
    };

    class knight : public chess_piece
    {
    public:
        // Constructors
        knight() : chess_piece() {}
        knight(color white_or_black, int id) : chess_piece(white_or_black, id, 'N') {}
        // Destructor
        virtual ~knight(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

        std::shared_ptr<chess_piece> clone() const {
            return std::make_shared<knight>(pcs::knight(this->piece_color, this->piece_id));
        }
    };

    class bishop : public chess_piece
    {
    public:
        // Constructors
        bishop() : chess_piece() {}
        bishop(color white_or_black, int id) : chess_piece(white_or_black, id, 'B') {}
        // Destructor
        virtual ~bishop(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

        std::shared_ptr<chess_piece> clone() const {
            return std::make_shared<bishop>(pcs::bishop(this->piece_color, this->piece_id));
        }
    };

    class queen : public chess_piece
    {
    public:
        // Constructors
        queen() : chess_piece() {}
        queen(color white_or_black, int id) : chess_piece(white_or_black, id, 'Q') {}
        // Destructor
        virtual ~queen(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

        std::shared_ptr<chess_piece> clone() const {
            return std::make_shared<queen>(pcs::queen(this->piece_color, this->piece_id));
        }
    };

    class king : public chess_piece
    {
    public:
        // Constructors
        king() : chess_piece() {}
        king(color white_or_black, int id) : chess_piece(white_or_black, id, 'K') {}
        // Destructor
        virtual ~king(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::shared_ptr<pcs::chess_piece>> chess_board);

        std::shared_ptr<chess_piece> clone() const {
            return std::make_shared<king>(pcs::king(this->piece_color, this->piece_id));
        }
    };
}

#endif