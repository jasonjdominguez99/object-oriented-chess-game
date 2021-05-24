// OOPCpp_Final_Assignment_Chess/chess_pieces.h
//
// Header file for chess_piece class
// and derived classes 
// 
// Author: Jason Dominguez


#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H


#include <iostream>
#include <vector>
#include <tuple>
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
        friend std::ostream & operator<<(std::ostream &output, std::unique_ptr<chess_piece> const& piece);

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
            piece_id = 0; 
            piece_symbol = ' ';
            has_moved = false;
            en_passant_is_possible = false;
        }
        chess_piece(color white_or_black, int id, char symbol): piece_color{white_or_black}, piece_id{id}, 
                                                                piece_symbol{symbol}, has_moved{false} {}
        chess_piece(color white_or_black, int id, char symbol, bool moved): piece_color{white_or_black}, piece_id{id}, 
                                                                            piece_symbol{symbol}, has_moved{moved} {}
        chess_piece(color white_or_black, int id, char symbol, bool moved, bool en_passant): piece_color{white_or_black}, piece_id{id}, 
                                                                                             piece_symbol{symbol}, has_moved{moved},
                                                                                             en_passant_is_possible{en_passant} {}
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
        
        virtual std::unique_ptr<chess_piece> clone() const = 0;
        virtual std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board)=0;
    };


    std::tuple<std::vector<int>,
               std::vector<std::vector<int>>,
               std::vector<int>> get_all_possible_moves(color piece_color, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

    void look_for_horizontal_and_vertical_moves(color piece_color,
                                                std::vector<int>& valid_new_positions, 
                                                int start_position_col, int start_position_row, 
                                                std::vector<std::unique_ptr<chess_piece>>const& chess_board);
                                                
    void look_for_diagonal_moves(color piece_color,
                                 std::vector<int>& valid_new_positions, 
                                 int start_position_col, int start_position_row, 
                                 std::vector<std::unique_ptr<chess_piece>>const& chess_board);


    class pawn : public chess_piece
    {
    public:
        // Constructors
        pawn() : chess_piece() {}
        pawn(color white_or_black, int id) : chess_piece(white_or_black, id, 'p') {}
        pawn(color white_or_black, int id, bool moved) : chess_piece(white_or_black, id, 'p', moved) {}
        pawn(color white_or_black, int id, bool moved, bool en_passant) : chess_piece(white_or_black, id, 'p', moved, en_passant) {}
        // Destructor
        virtual ~pawn(){}

        // Member functions
        std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

        std::unique_ptr<chess_piece> clone() const {
            return std::move(std::make_unique<pawn>(pcs::pawn(this->piece_color, this->piece_id, 
                                                              this->has_moved, this->en_passant_is_possible)));
        }
    };


    class rook : public chess_piece
    {  
    public:
        // Constructors
        rook() : chess_piece() {}
        rook(color white_or_black, int id) : chess_piece(white_or_black, id, 'R') {}
        rook(color white_or_black, int id, bool moved) : chess_piece(white_or_black, id, 'R', moved) {}
        // Destructor
        virtual ~rook(){}

        // Member functions
        std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

        std::unique_ptr<chess_piece> clone() const {
            return std::move(std::make_unique<rook>(pcs::rook(this->piece_color, this->piece_id, this->has_moved)));
        }
    };


    class knight : public chess_piece
    {
    public:
        // Constructors
        knight() : chess_piece() {}
        knight(color white_or_black, int id) : chess_piece(white_or_black, id, 'N') {}
        knight(color white_or_black, int id, bool moved) : chess_piece(white_or_black, id, 'N', moved) {}
        // Destructor
        virtual ~knight(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

        std::unique_ptr<chess_piece> clone() const {
            return std::move(std::make_unique<knight>(pcs::knight(this->piece_color, this->piece_id, this->has_moved)));
        }
    };


    class bishop : public chess_piece
    {
    public:
        // Constructors
        bishop() : chess_piece() {}
        bishop(color white_or_black, int id) : chess_piece(white_or_black, id, 'B') {}
        bishop(color white_or_black, int id, bool moved) : chess_piece(white_or_black, id, 'B', moved) {}
        // Destructor
        virtual ~bishop(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

        std::unique_ptr<chess_piece> clone() const {
            return std::move(std::make_unique<bishop>(pcs::bishop(this->piece_color, this->piece_id, this->has_moved)));
        }
    };


    class queen : public chess_piece
    {
    public:
        // Constructors
        queen() : chess_piece() {}
        queen(color white_or_black, int id) : chess_piece(white_or_black, id, 'Q') {}
        queen(color white_or_black, int id, bool moved) : chess_piece(white_or_black, id, 'Q', moved) {}
        // Destructor
        virtual ~queen(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

        std::unique_ptr<chess_piece> clone() const {
            return std::move(std::make_unique<queen>(pcs::queen(this->piece_color, this->piece_id, this->has_moved)));
        }
    };


    class king : public chess_piece
    {
    public:
        // Constructors
        king() : chess_piece() {}
        king(color white_or_black, int id) : chess_piece(white_or_black, id, 'K') {}
        king(color white_or_black, int id, bool moved) : chess_piece(white_or_black, id, 'K', moved) {}
        // Destructor
        virtual ~king(){}

        std::vector<int> get_valid_moves(int start_position, std::vector<std::unique_ptr<pcs::chess_piece>> chess_board);

        std::unique_ptr<chess_piece> clone() const {
            return std::move(std::make_unique<king>(pcs::king(this->piece_color, this->piece_id, this->has_moved)));
        }
    };
}

#endif