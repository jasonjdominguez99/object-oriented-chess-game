// OOPCpp_Final_Assignment_Chess/main.cpp
//
// Main chess game program (previously chess_game.cpp)
// 
// Author: Jason Dominguez
// Created: 27/04/2021
// Last modified: 
// 27/04/2021
// - Changed file organization to have a main.cpp
//   and separate chess_game.hpp and chess_game.cpp files
// 30/04/2021
// - Restructured entire code to include chess_game class
//   still capable of the same functionality, but now
//   able to start on chess engine


#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <memory>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_board.hpp"
#include "chess_game.hpp"


using namespace plr;
using namespace pcs;
using namespace brd;
using namespace cgm;


int main() {
    
    std::cout << "Please enter the number of players (1/2): ";
    int number_of_players;
    std::cin >> number_of_players;
    
    human_player player_one;
    std::cout << "Player 1" << std::endl;
    player_one.ask_for_name();
    player_one.ask_for_color();
    std::cout << player_one.get_name() << " your color is " << color_to_string(player_one.get_piece_color()) << std::endl;

    std::shared_ptr<player> player_two;
    if (number_of_players == 1) {
        player_two = std::make_shared<chess_bot>(opposite_color(player_one.get_piece_color()));
    } else {
        std::cout << "Player 2" << std::endl;
        player_two = std::make_shared<human_player>(opposite_color(player_one.get_piece_color()));
        player_two->ask_for_name();
        std::cout << player_two->get_name() << " your color is " << color_to_string(player_two->get_piece_color()) << std::endl;
    }

    chess_game game(player_one, player_two);
    std::cout << std::endl << game.get_chess_board() << std::endl;

    while (game.has_ended() == false) {
        bool in_check;
        if (game.get_game_status() == check) {
            std::cout << "QUICK! " << game.get_current_player()->get_name() << ", you must move your must move your king!" << std::endl;
            in_check = true;
        } else {
            in_check = false;
        }

        game.current_player_make_a_move(in_check);
        std::cout << std::endl << game.get_chess_board() << std::endl;

        game.update_game_status();
        game.get_next_player_ready();
    }

    std::cout << "Game over!" << std::endl;

    return 0;
}