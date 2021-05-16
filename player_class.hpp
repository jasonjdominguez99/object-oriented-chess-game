// OOPCpp_Final_Assignment_Chess/player_class.hpp
//
// Header file for player class
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified:
// 30/04/2021
// - Restructured entire code to include chess_game class
//   still capable of the same functionality, but now
//   able to start on chess engine
// 01/05/2021
// - Added chess_bot functionality to randomly select
//   possible moves using random seed
// 06/05/2021
// - Removed using namespace to adhere to house style
// 15/05/2021
// - Added separate function to return all valid moves for a player


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
        player(std::string player_name, pcs::color player_color, bool human_or_not) :
            name{player_name}, piece_color{player_color}, is_human{human_or_not} {}
        player(player &player_to_copy) {
            name = player_to_copy.name;
            piece_color = player_to_copy.piece_color;
        }
        // Destructor
        virtual ~player(){}

        // Member functions
        virtual std::pair<int, int> choose_move(brd::board& chess_board)=0;
        virtual std::pair<int, int> choose_move_for_king(brd::board& chess_board)=0;
        std::vector<std::pair<int, std::vector<int>>> get_player_possible_moves(brd::board& chess_board);
        std::vector<std::pair<int, std::vector<int>>> get_player_piece_possible_moves(char chess_piece, brd::board& chess_board);
        std::pair<int, std::vector<int>> get_piece_valid_moves(int start_position_index, std::vector<int> possible_final_positions, brd::board& chess_board);
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
        std::pair<int, int> choose_move(brd::board& chess_board);
        std::pair<int, int> choose_move_for_king(brd::board& chess_board);
    };

    class chess_bot : public player
    {
    public:
        // Constructors
        chess_bot(pcs::color player_color) : player("ChessBot", player_color, false) {} 
        // Destructor
        virtual ~chess_bot(){}

        // Member functions
        std::pair<int, int> choose_move(brd::board& chess_board);
        std::pair<int, int> choose_move_for_king(brd::board& chess_board);
    };
}

#endif