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


#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include "chess_pieces.hpp"
#include "chess_board.hpp"


using namespace pcs;
using namespace brd;


namespace plr {

    class player
    {
    protected:
        std::string name{};
        color piece_color{};
        bool is_human{};

    public:
        // Constructors
        player() {}
        player(color player_color) : piece_color{player_color} {}
        player(bool human_or_not) : is_human{human_or_not} {}
        player(std::string player_name, color player_color, bool human_or_not) :
            name{player_name}, piece_color{player_color}, is_human{human_or_not} {}
        player(player &player_to_copy) {
            name = player_to_copy.name;
            piece_color = player_to_copy.piece_color;
        }
        // Destructor
        virtual ~player(){}

        // Member functions
        virtual std::pair<int, int> choose_move(board& chess_board)=0;
        std::pair<int, int> choose_move_for_king(board& chess_board);
        void ask_for_name();
        void ask_for_color();

        std::string get_name() { return name; }
        color get_piece_color() { return piece_color; }
        bool get_is_human() { return is_human; }

        /*
        player & operator=(player &player_to_copy) {
            if(&player_to_copy == this) return *this; // account for self-assignment
         
            name = player_to_copy.name; 
            piece_color = player_to_copy.piece_color;

            return *this;
        }
        */
    };

    class human_player : public player
    {
    public:
        // Constructors
        human_player() { is_human = true; }
        human_player(color player_color) : player(player_color) {} 
        human_player(bool human_or_not) : player(human_or_not) {}
        // Destructor
        virtual ~human_player(){}

        // Member functions
        std::pair<int, int> choose_move(board& chess_board);
    };

    class chess_bot : public player
    {
    public:
        // Constructors
        chess_bot(color player_color) : player("ChessBot", player_color, false) {} 
        // Destructor
        virtual ~chess_bot(){}

        // Member functions
        std::pair<int, int> choose_move(board& chess_board);
    };
}

#endif