// OOPCpp_Final_Assignment_Chess/main.cpp
//
// Main chess game program (previously chess_game.cpp)
// 
// Author: Jason Dominguez


#include <string>
#include <memory>
#include <limits>
#include "chess_pieces.hpp"
#include "player_class.hpp"
#include "chess_game.hpp"


// Function declarations
int get_valid_input(int valid_input_one, int valid_input_two);
bool ask_for_yes_or_no(std::string question, std::string invalid_input_response);


int main() {
    std::cout << "Please enter the number of players (1/2): ";
    int number_of_players{};
    number_of_players = get_valid_input(1, 2);
    
    plr::human_player player_one;
    std::cout << "Player 1" << std::endl;
    player_one.ask_for_name();
    player_one.ask_for_color();
    std::cout << player_one.get_name() << " your color is " << pcs::color_to_string(player_one.get_piece_color()) << std::endl;

    std::shared_ptr<plr::player> player_two;
    if (number_of_players == 1) {
        player_two = std::make_shared<plr::chess_bot>(pcs::opposite_color(player_one.get_piece_color()));
    } else {
        std::cout << "Player 2" << std::endl;
        player_two = std::make_shared<plr::human_player>(pcs::opposite_color(player_one.get_piece_color()));
        player_two->ask_for_name();
        std::cout << player_two->get_name() << " your color is " << pcs::color_to_string(player_two->get_piece_color()) << std::endl;
    }

    bool keep_playing{true};
    while (keep_playing) {
        cgm::chess_game game(player_one, player_two);

        // Make sure that the white player goes first
        if (player_one.get_piece_color() == pcs::black) {
            game.get_next_player_ready();
        }
                
        // Load a saved game if required by user
        bool load_game = ask_for_yes_or_no("Would you like to load a saved game? (y/n) ",
                                           "Were you not listening...? I said, WANT TO LOAD A GAME?!");
        if (load_game) {
            game.load_game();
        }
        
        std::cout << std::endl << game.get_chess_board() << std::endl;

        bool quit_game{false};
        while (game.has_ended() == false && quit_game == false) {
            bool in_check;
            if (game.get_game_status() == cgm::check) {
                std::cout << "QUICK! " << game.get_current_player()->get_name() 
                          << ", you must move your must protect your king!" << std::endl;
                in_check = true;
            } else {
                in_check = false;
            }
            
            quit_game = game.current_player_make_a_move(in_check);
            if (quit_game) { break; }
            std::cout << std::endl << game.get_chess_board() << std::endl;

            game.promote_pawn_if_possible();

            game.update_game_status();
            game.get_next_player_ready();
        }

        game.game_over();

        bool save_game{};
        save_game = ask_for_yes_or_no("Would you like to save this game? (y/n) ",
                                      "Were you not listening...? I said, WOULD YOU LIKE TO SAVE THIS GAME?!");
        if (save_game) {
            game.save_game();
        }

        keep_playing = false;
        keep_playing = ask_for_yes_or_no("Fancy another game? (y/n) ",
                                         "Were you not listening...? I said, FANCY ANOTHER GAME?!");

    }

    cgm::chess_game::display_stats(player_one.get_name(), player_two->get_name());

    return 0;
}


// Function definitions
int get_valid_input(int valid_input_one, int valid_input_two) {
    int number_input{};
    std::string input{};
    std::getline(std::cin, input);

    bool is_input_valid{false};
    while (!is_input_valid) {
        // Make sure input is one of two valid inputs and int
        if (std::cin.fail() || input == "" || input.find_first_not_of("0123456789") != std::string::npos || input.size() >= 10) {
            std::cin.clear();
            std::cout << "Sorry, your input was not valid, please enter "
                         "a valid input (" << valid_input_one << "/" <<
                         valid_input_two << "): ";
            std::getline(std::cin, input);
        } else {
            // Make sure integer value is one of the valid inputs
            number_input = std::stoi(input);
            if (number_input != valid_input_one && number_input != valid_input_two) {
                std::cin.clear();
                std::cout << "Sorry, your input was not valid, please enter a valid input (" << valid_input_one 
                          << "/" << valid_input_two << "): ";
                std::getline(std::cin, input);
            } else {
                is_input_valid = true;
            }
        }
    }
    return number_input;
}


bool ask_for_yes_or_no(std::string question, std::string invalid_input_response) {
    while (true) {
        std::cout << question;
        std::string save_game_input;
        std::cin >> save_game_input;
        if (save_game_input == "y" || save_game_input == "Y" || save_game_input == "yes" || save_game_input == "Yes") {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        } else if (save_game_input == "n" || save_game_input == "N" || save_game_input == "no" || save_game_input == "No") {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        } else {
            std::cout << invalid_input_response << std::endl;
            std::cout << "Let's try this again..." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } 
}