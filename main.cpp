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
// 02/05/2021
// - Added functionality to promote pawns


#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <memory>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_game.hpp"


using namespace plr;
using namespace pcs;
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

    bool keep_playing{true};
    while (keep_playing) {
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

            game.promote_pawn_if_possible();

            game.update_game_status();
            game.get_next_player_ready();
        }

        game.game_over();

        keep_playing = false;
        while (true) {
            std::cout << "Fancy another game? (y/n) ";
            std::string keep_playing_input;
            std::cin >> keep_playing_input;
            if (keep_playing_input == "y" || keep_playing_input == "Y" ||
                keep_playing_input == "yes" || keep_playing_input == "Yes") {

                keep_playing = true;
                break;
            } else if (keep_playing_input == "n" || keep_playing_input == "N" ||
                       keep_playing_input == "no" || keep_playing_input == "No") {
            
                keep_playing = false;
                break;
            } else {
                std::cout << "Were you not listening...? I said, FANCY ANOTHER GAME!" << std::endl;
                std::cout << "Let's try this again..." << std::endl;
            }
        }
    }

    chess_game::display_stats(player_one.get_name(), player_two->get_name());

    return 0;
}