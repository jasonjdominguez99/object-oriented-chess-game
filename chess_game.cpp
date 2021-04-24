// OOPCpp_Final_Assignment_Chess/chess_game.cpp
//
// Source file for main chess game program
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 23/04/2021


#include <string>
#include <vector>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_board.hpp"


using namespace plr;
using namespace pcs;
using namespace brd;


// Function declarations
std::string ask_for_name();
color ask_for_color(std::string player_name);
void swap_player(player& current_player, player player_one, player player_two);
void player_turn(player chess_player, board& chess_board);


int main() {
    
    std::cout << "Please enter the number of players (1/2): ";
    int number_of_players;
    std::cin >> number_of_players;

    if (number_of_players == 1) {
        std::cout << "AI has not yet been developed" << std::endl;
    } else if (number_of_players == 2) {
        std::cout << std::endl << "Player 1" << std::endl;
        std::string player_one_name = ask_for_name();
        color player_one_color = ask_for_color(player_one_name);
        player player_one(player_one_name, player_one_color);
        std::cout << player_one.get_name() << " your color is " << color_to_string(player_one.get_piece_color()) << std::endl;

        std::cout << std::endl << "Player 2" << std::endl;
        std::string player_two_name = ask_for_name();
        color player_two_color = opposite_color(player_one_color);
        player player_two(player_two_name, player_two_color);
        std::cout << player_two.get_name() << " your color is " << color_to_string(player_two.get_piece_color()) << std::endl;

        board chess_board;
        std::cout << chess_board << std::endl;

        bool game_has_ended = false;
        player current_player = player_one;
        while (!game_has_ended) {
            player_turn(current_player, chess_board);

            std::cout << std::endl << chess_board << std::endl;

            swap_player(current_player, player_one, player_two);
        }
    }
    
    return 0;
}


// Function definitions
std::string ask_for_name() {
    std::cout << "Please enter your name: ";
    std::string name;
    std::cin >> name;

    return name;
}

color ask_for_color(std::string player_name) {
    std::cout << player_name << " please enter your chess piece color: ";
    std::string chess_piece_color;
    std::cin >> chess_piece_color;
    color piece_color = string_to_color(chess_piece_color);

    return piece_color;
}

void swap_player(player& current_player, player player_one, player player_two) {
    if (current_player.get_piece_color() == player_one.get_piece_color()) {
        current_player = player_two;
    } else {
        current_player = player_one;
    }
}

void player_turn(player chess_player, board& chess_board) {
    std::cout << chess_player.get_name() << "'s turn..." << std::endl;

    // Initialize to a poistion on chess board with opposite color of player
    int start_position_index{};
    if (chess_player.get_piece_color() == white) {
        start_position_index = 63;
    } else {
        start_position_index = 0;
    }
    
    while (true) {
        std::vector<int> possible_moves{};
        while (possible_moves.size() == 0) {
            while (chess_board[start_position_index]->get_piece_color() != chess_player.get_piece_color()) {
                start_position_index = 16; // Set to a position with no chess piece
                while (!chess_board[start_position_index]) {
                    std::cout << "Please enter the position of the chess piece you want to move: " ;
                    std::string start_position;
                    std::cin >> start_position;
                    start_position_index = board_position_to_index(start_position);

                    if (!chess_board[start_position_index]) {
                        std::cout << "Ummm... that's... not a chess piece...... Please try again..." << std::endl;
                    }
                }
                if (chess_board[start_position_index]->get_piece_color() != chess_player.get_piece_color()) {
                    std::cout << "You can only move a " << color_to_string(chess_player.get_piece_color()) << " chess piece. You're not trying to cheat are you...?" << std::endl;
                }
            }
            // Get the possible moves for the chosen piece
            possible_moves = chess_board[start_position_index]->get_valid_moves(start_position_index, chess_board);
            if (possible_moves.size() == 0) {
                std::cout << "Sorry, this piece has no possible moves. Please try again..." << std::endl;
                if (chess_player.get_piece_color() == white) {
                    start_position_index = 63;
                } else {
                    start_position_index = 0;
                }
            }
        }

        std::cout << "Here are the possible moves for this chess piece: " << std::endl;
        std::vector<int>::iterator vector_begin{possible_moves.begin()};
        std::vector<int>::iterator vector_end{possible_moves.end()};
        std::vector<int>::iterator vector_iterator;
        for (vector_iterator = vector_begin ; vector_iterator < vector_end ; ++vector_iterator) {
            std::cout << board_index_to_position(*vector_iterator) << std::endl;
        }
    
        std::cout << "Do you still want to move this chess piece (y/n)?" << std::endl;
        std::string piece_to_move_decision;
        std::cin >> piece_to_move_decision;
        if (piece_to_move_decision == "y") {
            break;
        } else {
            if (chess_player.get_piece_color() == white) {
                start_position_index = 63;
            } else {
                start_position_index = 0;
            }
        }
    }

    std::cout << "To which of these positions do you want to move your piece?" << std::endl;
    std::string end_position;
    std::cin >> end_position;
    int end_position_index = board_position_to_index(end_position);

    chess_board.move_piece(start_position_index, end_position_index);
}