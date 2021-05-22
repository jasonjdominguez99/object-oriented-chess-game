// OOPCpp_Final_Assignment_Chess/player_class.hpp
//
// Header file for player class
// 
// Author: Jason Dominguez


#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <utility>
#include "chess_pieces.hpp"
#include "chess_board.hpp"


// Class and function definitions
namespace plr {

    class player
    {
    protected:
        std::string name{};
        pcs::color piece_color{};
        bool is_human{};

    public:
        // Constructors
        player() {}
        player(pcs::color player_color) : piece_color{player_color} {}
        player(bool human_or_not) : is_human{human_or_not} {}
        player(std::string player_name, pcs::color player_color, bool human_or_not) : name{player_name}, 
                                                                                      piece_color{player_color}, 
                                                                                      is_human{human_or_not} {}
        player(player &player_to_copy) {
            name = player_to_copy.name;
            piece_color = player_to_copy.piece_color;
        }
        // Destructor
        virtual ~player(){}

        // Member functions
        virtual std::pair<int, int> choose_move(const brd::board& chess_board)=0;
        std::vector<std::pair<int, std::vector<int>>> get_player_possible_moves(const brd::board& chess_board);
        std::vector<std::pair<int, std::vector<int>>> get_player_piece_possible_moves(char chess_piece, 
                                                                                      const brd::board& original_chess_board);
        std::pair<int, std::vector<int>> get_piece_valid_moves(int start_position_index, 
                                                               std::vector<int> possible_final_positions, 
                                                               const brd::board& chess_board);
        void ask_for_name();
        void ask_for_color();

        std::string get_name() { return name; }
        pcs::color get_piece_color() { return piece_color; }
        bool get_is_human() { return is_human; }
    };


    class human_player : public player
    {
    public:
        // Constructors
        human_player() { is_human = true; }
        human_player(pcs::color player_color) : player(player_color) {} 
        human_player(bool human_or_not) : player(human_or_not) {}
        // Destructor
        virtual ~human_player(){}

        // Member functions
        std::pair<int, int> choose_move(const brd::board& chess_board);
    };


    class chess_bot : public player
    {
    public:
        // Constructors
        chess_bot(pcs::color player_color) : player("ChessBot", player_color, false) {} 
        // Destructor
        virtual ~chess_bot(){}

        // Member functions
        std::pair<int, int> choose_move(const brd::board& chess_board);
    };
}

#endif