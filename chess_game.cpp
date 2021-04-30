// OOPCpp_Final_Assignment_Chess/chess_game.cpp
//
// Source file for chess game class
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified: 
// 24/04/2021
// - Added functionality to see if check has occurred
// - Added functionality to see if checkmate has occurred
// 25/04/2021
// - Added functionality for game to end upon checkmate
// - Added functionality to force player to move king 
//   when in check
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
#include <utility>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_board.hpp"
#include "chess_game.hpp"


using namespace plr;
using namespace pcs;
using namespace brd;
using namespace cgm;


// Function definitions
void chess_game::get_next_player_ready() {
    if (current_player == players[0]) {
        current_player = players[1];
    } else {
        current_player = players[0];
    }
    
    /*
    std::vector<std::shared_ptr<plr::player>>::iterator first_player{players.begin()};
    std::vector<std::shared_ptr<plr::player>>::iterator last_player{players.end()};
    std::vector<std::shared_ptr<plr::player>>::iterator chess_player;
    for (chess_player = first_player; chess_player < last_player ; ++chess_player) {
        if (current_player != *chess_player) {
            current_player = *chess_player;
        }
    }
    */
}

void chess_game::current_player_make_a_move(bool in_check) {
    std::pair<int, int> move;
    if (in_check) {
        move = current_player->choose_move_for_king(chess_board);
    } else {
        move = current_player->choose_move(chess_board);
    }
    int start_position_index = move.first;
    int end_position_index = move.second;

    chess_board.move_piece(start_position_index, end_position_index);
    chess_board[end_position_index]->has_been_moved();
}

void chess_game::update_game_status() {
    bool check_on_opposition;
    bool checkmate_on_opposition;
    std::vector<int> possible_next_moves;

    // See if check has occured (current player can capture the opposition king on their next turn)
    possible_next_moves = get_all_possible_moves(current_player->get_piece_color(), chess_board.get_board());
    // Find if any of these moves will capture the opposition's king
    check_on_opposition = std::find_if(possible_next_moves.begin(), possible_next_moves.end(),
                                       [this](int board_index) {
                                           if (this->chess_board[board_index]) {
                                               return this->chess_board[board_index]->get_symbol() == 'K' && this->chess_board[board_index]->get_piece_color() != this->current_player->get_piece_color();
                                           }
                                       }) != possible_next_moves.end();
    
    // See if checkmate has occured (this player will capture the opposition king on their next turn)
    if (check_on_opposition) {
        int opposition_king_board_index{};
        for (int i{} ; i < 8*8 ; i++) {
            if (chess_board[i] && 
                chess_board[i]->get_piece_color() != current_player->get_piece_color() &&
                chess_board[i]->get_symbol() == 'K') {
                            
                opposition_king_board_index = i;
            }
        }
        std::vector<int> opposition_king_possible_moves;
        opposition_king_possible_moves = chess_board[opposition_king_board_index]->get_valid_moves(opposition_king_board_index, chess_board.get_board());
        if (opposition_king_possible_moves.size() == 0) {
            // Opposition king cannot escape getting captured
            checkmate_on_opposition = true;
        }
    }
    // Update the status of the game if check or checkmate has occurred
    if (check_on_opposition && !checkmate_on_opposition) {
        std::cout << "Check!" << std::endl;
        status = check;
    } else if (checkmate_on_opposition) {
        std::cout << "Checkmate!" << std::endl;
        status = checkmate;
    } else {
        status = active;
    }
}