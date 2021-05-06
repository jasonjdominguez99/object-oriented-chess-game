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
// 02/05/2021
// - Added functionality to promote pawn
// - Added compatibility with choosing legal en passant
//   and castling moves
// 06/05/2021
// - Removed using namespace to adhere to house style


#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_board.hpp"
#include "chess_game.hpp"


// Class and function definitions
int cgm::chess_game::games_played{0};
int cgm::chess_game::player_one_wins{0};
int cgm::chess_game::player_two_wins{0};
int cgm::chess_game::draws{0};

void cgm::chess_game::get_next_player_ready() {
    if (current_player == players[0]) {
        current_player = players[1];
    } else {
        current_player = players[0];
    }
}

void cgm::chess_game::current_player_make_a_move(bool in_check) {
    std::pair<int, int> move;
    if (in_check) {
        move = current_player->choose_move_for_king(chess_board);
    } else {
        move = current_player->choose_move(chess_board);
    }
    int start_position_index = move.first;
    int end_position_index = move.second;

    brd::move_type type_of_move{brd::standard};
    // Must be en passant move if pawn is moving diagonally into empty square
    if (chess_board[start_position_index]->get_symbol() == 'p' && !chess_board[end_position_index]) {
        if (end_position_index == start_position_index + 8 + 1 || end_position_index == start_position_index + 8 - 1 || 
            end_position_index == start_position_index - 8 + 1 || end_position_index == start_position_index - 8 - 1) {
            
            type_of_move = brd::en_passant;
        }
    } else if (chess_board[start_position_index]->get_symbol() == 'K' && !chess_board[end_position_index]) {
        if (end_position_index == start_position_index + 2 || end_position_index == start_position_index - 2) {
            type_of_move = brd::castling;
        }
    }
    
    chess_board.move_piece(start_position_index, end_position_index, type_of_move);

    // Reset all pawns to have no possibility of being captured via en passant
    for (int i{} ; i < 8*8 ; i++) {
        if (chess_board[i] && chess_board[i]->get_symbol() == 'p') {
            chess_board[i]->set_en_passant_possibility(false);
        }
    }
    // If pawn was just moved by two and it was its first move, 
    // it can be captured via en passant
    if (chess_board[end_position_index]->get_symbol() == 'p' &&
        chess_board[end_position_index]->get_has_moved() == false) {
        
        int pawn_position_row{end_position_index/8};
        int fourth_row_index{3};
        int fifth_row_index{4};
        if (chess_board[end_position_index]->get_piece_color() == pcs::white &&
            pawn_position_row == fourth_row_index) {
            
            chess_board[end_position_index]->set_en_passant_possibility(true);
        } else if (chess_board[end_position_index]->get_piece_color() == pcs::black &&
                   pawn_position_row == fifth_row_index) {
            
            chess_board[end_position_index]->set_en_passant_possibility(true);
        }

    }
    // Setting has_been_moved must be after checking for en passant possibility as
    // only pawns whose first move was two steps can be captured en passant, 
    // so need to know if pawn had moved before this move
    chess_board[end_position_index]->has_been_moved();
    // For castling rook has also been moved
    if (type_of_move == brd::castling) {
        if (end_position_index > start_position_index) {
            chess_board[end_position_index - 1]->has_been_moved();
        } else {
            chess_board[end_position_index + 1]->has_been_moved();
        }
    }
}

void cgm::chess_game::promote_pawn_if_possible() {
    int opposite_row_begin_index;
    int opposite_row_end_index;
    if (current_player->get_piece_color() == pcs::white) {
        opposite_row_begin_index = 57;
        opposite_row_end_index = 64;
    } else {
        opposite_row_begin_index = 0;
        opposite_row_end_index = 8;
    }

    for (int i{opposite_row_begin_index} ; i < opposite_row_end_index ; i++) {
        if (chess_board[i] && 
            chess_board[i]->get_symbol() == 'p' && 
            chess_board[i]->get_piece_color() == current_player->get_piece_color()) {
            
            int pawn_to_promote_index = i;
            int old_pawn_id = chess_board[pawn_to_promote_index]->get_id();
            std::cout << current_player->get_name() << ", you must promote your pawn, well done!" << std::endl;

            while (true) {
                std::cout << "Please choose which of these pieces to which you want to promote your pawn: (N, B, R, Q)" << std::endl;
                
                std::string chosen_promotion_piece_symbol;
                std::cin >> chosen_promotion_piece_symbol;

                if (chosen_promotion_piece_symbol == "N") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_shared<pcs::knight>(pcs::knight(current_player->get_piece_color(), old_pawn_id)));
                    break;
                } else if (chosen_promotion_piece_symbol == "B") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_shared<pcs::bishop>(pcs::bishop(current_player->get_piece_color(), old_pawn_id)));
                    break;
                } else if (chosen_promotion_piece_symbol == "R") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_shared<pcs::rook>(pcs::rook(current_player->get_piece_color(), old_pawn_id)));
                    break;
                } else if (chosen_promotion_piece_symbol == "Q") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_shared<pcs::queen>(pcs::queen(current_player->get_piece_color(), old_pawn_id)));
                    break;
                } else {
                    std::cout << "You MUST promote to one of these pieces (N, B, R, Q)" << std::endl;
                    std::cout << "Let's try this again..." << std::endl;
                }
            }

            chess_board[pawn_to_promote_index]->has_been_moved();
            std::cout << std::endl << this->get_chess_board() << std::endl;
            break;
        }
    }
}

void cgm::chess_game::update_game_status() {
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
                                           } else {
                                               return false;
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
                break;
            }
        }
        std::vector<int> opposition_king_possible_moves;
        opposition_king_possible_moves = chess_board[opposition_king_board_index]->get_valid_moves(opposition_king_board_index, chess_board.get_board());
        
        // See if king has possible moves to escape capture
        std::vector<int>::iterator first_new_position{opposition_king_possible_moves.begin()};
        std::vector<int>::iterator last_new_position{opposition_king_possible_moves.end()};
        std::vector<int>::iterator possible_final_position;
        std::vector<int> moves_to_remove;
        for (possible_final_position = first_new_position ; possible_final_position < last_new_position ; ++possible_final_position) {
            // Make each of the possible king moves on a copy of the current chess board
            std::vector<std::shared_ptr<pcs::chess_piece>> chess_board_after_possible_next_move{chess_board.get_board()};

            chess_board_after_possible_next_move[*possible_final_position] = std::move(chess_board_after_possible_next_move[opposition_king_board_index]);
            chess_board_after_possible_next_move[*possible_final_position]->has_been_moved();

            std::vector<int> opposition_possible_moves = get_all_possible_moves(this->current_player->get_piece_color(), chess_board_after_possible_next_move);

            // If this possible final move for the king was found in possible opposition moves then
            // it can be captured, therefore we will remove this from possible moves for this king
            if (std::find(opposition_possible_moves.begin(), 
                          opposition_possible_moves.end(),
                          *possible_final_position) != opposition_possible_moves.end()) {
            
                moves_to_remove.push_back(*possible_final_position);
            }
        }  
    
        // Remove this possible position for the king, if found in opposition possible moves
        opposition_king_possible_moves.erase(std::remove_if(opposition_king_possible_moves.begin(), 
                                                            opposition_king_possible_moves.end(),
                                                            [&moves_to_remove](const int& position) -> bool {
                                                                return std::find(moves_to_remove.begin(),
                                                                                 moves_to_remove.end(),
                                                                                 position) != moves_to_remove.end();
                                                            }), opposition_king_possible_moves.end());
        
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

void cgm::chess_game::game_over() {
    std::cout << "Game over!" << std::endl;

    ++games_played;
    if (current_player == players[0] && this->status == checkmate) {
        std::cout << "Winner: " << players[1]->get_name() << std::endl;
        std::cout << "Loser: " << players[0]->get_name() << std::endl;
        ++player_two_wins;
    } else if (current_player == players[1] && this->status == checkmate) {
        std::cout << "Winner: " << players[0]->get_name() << std::endl;
        std::cout << "Loser: " << players[1]->get_name() << std::endl;
        ++player_one_wins;
    } else {
        std::cout << "Draw" << std::endl;
        ++draws;
    }
}

void cgm::chess_game::display_stats(std::string player_one_name, std::string player_two_name) {
    std::cout << std::endl << "        Final game statistics" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Games played: " << games_played << std::endl;
    std::cout << player_one_name << ": " << player_one_wins << std::endl;
    std::cout << player_two_name << ": " << player_two_wins << std::endl;
    std::cout << "Games drawn: " << draws << std::endl;
    if (player_one_wins > player_two_wins) {
        std::cout << player_one_name << " is the overall winner, congrats!" << std::endl;
    } else if (player_one_wins < player_two_wins) {
        std::cout << player_two_name << " is the overall winner, congrats!" << std::endl;
    } else {
        std::cout << "Overal a draw... pretty boring..." << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
}