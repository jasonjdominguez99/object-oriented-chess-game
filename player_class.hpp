// OOPCpp_Final_Assignment_Chess/player_class.h
//
// Header file for player class
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 23/04/2021


#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include "chess_pieces.hpp"


using namespace pcs;


namespace plr {
    class player
    {
    private:
        std::string name{};
        color piece_color{};

    public:
        // Constructors
        player(std::string player_name, color player_color) :
               name{player_name}, piece_color{player_color} {}
        player(player &player_to_copy) {
            name = player_to_copy.name;
            piece_color = player_to_copy.piece_color;
        }
        // Destructor
        ~player(){}

        // Member functions
        std::string get_name() { return name; }
        color get_piece_color() { return piece_color; }
        player & operator=(player &player_to_copy) {
            if(&player_to_copy == this) return *this; // account for self-assignment
         
            name = player_to_copy.name; 
            piece_color = player_to_copy.piece_color;

            return *this;
        }
    };
}

#endif