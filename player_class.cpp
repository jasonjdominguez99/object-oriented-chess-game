// OOPCpp_Final_Assignment_Chess/player_class.cpp
//
// Source file for player class
// 
// Author: Jason Dominguez
// Created: 23/04/2021
// Last modified:
// 30/04/2021
// - Restructured entire code to include chess_game class
//   still capable of the same functionality, but now
//   able to start on chess engine


#include <algorithm>
#include <utility>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_board.hpp"


using namespace plr;
using namespace pcs;
using namespace brd;


void player::ask_for_name() {
    std::cout << "Please enter your name: ";
    std::string player_name;
    std::cin >> player_name;

    name = player_name;
}

void player::ask_for_color() {
    std::cout << name << " please enter your chess piece color: ";
    std::string chess_piece_color;
    std::cin >> chess_piece_color;

    piece_color = string_to_color(chess_piece_color);
}

std::pair<int, int> player::choose_move_for_king(board& chess_board) {
    int king_position_index{};
    for (int i{} ; i < 8*8 ; i++) {
        if (chess_board[i] && 
            chess_board[i]->get_piece_color() == this->get_piece_color() &&
            chess_board[i]->get_symbol() == 'K') {
                        
            king_position_index = i;
        }
    }
    std::vector<int> possible_moves{};
    possible_moves = chess_board[king_position_index]->get_valid_moves(king_position_index, chess_board.get_board());

    // For a king, must make sure that it is not being moved into check position
    if (chess_board[king_position_index]->get_symbol() == 'K') {
        std::vector<int>::iterator first_new_position{possible_moves.begin()};
        std::vector<int>::iterator last_new_position{possible_moves.end()};
        std::vector<int>::iterator possible_final_position;
        std::vector<int> moves_to_remove;
        for (possible_final_position = first_new_position ; possible_final_position < last_new_position ; ++possible_final_position) {
                    // Make each of the possible king moves on a copy of the current chess board
            std::vector<std::shared_ptr<pcs::chess_piece>> chess_board_after_possible_next_move{chess_board.get_board()};

            chess_board_after_possible_next_move[*possible_final_position] = std::move(chess_board_after_possible_next_move[king_position_index]);
            chess_board_after_possible_next_move[*possible_final_position]->has_been_moved();

            std::vector<int> opposition_possible_moves = get_all_possible_moves(opposite_color(this->get_piece_color()), chess_board_after_possible_next_move);

            // If this possible final move for the king was found in possible opposition moves then
            // it can be captured, therefore we will remove this from possible moves for this king
            if (std::find(opposition_possible_moves.begin(), 
                                  opposition_possible_moves.end(),
                                  *possible_final_position) != opposition_possible_moves.end()) {
            
                moves_to_remove.push_back(*possible_final_position);
            }
        }  
    
        // Remove this possible position for the king, if found in opposition possible moves
        possible_moves.erase(std::remove_if(possible_moves.begin(), 
                                                    possible_moves.end(),
                                                    [&moves_to_remove](const int& position) -> bool {
                                                        return std::find(moves_to_remove.begin(),
                                                                         moves_to_remove.end(),
                                                                         position) != moves_to_remove.end();
                                                    }), possible_moves.end());
    }

    std::cout << "Here are the possible moves for this chess piece: " << std::endl;
    std::vector<int>::iterator vector_begin{possible_moves.begin()};
    std::vector<int>::iterator vector_end{possible_moves.end()};
    std::vector<int>::iterator vector_iterator;
    for (vector_iterator = vector_begin ; vector_iterator < vector_end ; ++vector_iterator) {
        std::cout << board_index_to_position(*vector_iterator) << std::endl;
    }

    std::cout << "To which of these positions do you want to move your piece?" << std::endl;
    std::string end_position;
    std::cin >> end_position;
    int end_position_index = board_position_to_index(end_position);
    
    return std::pair<int, int>(king_position_index, end_position_index);
}

std::pair<int, int> human_player::choose_move(board& chess_board) {
    std::cout << name << "'s turn..." << std::endl;

    int start_position_index{};
    while (true) {
        std::vector<int> possible_moves{};
        while (possible_moves.size() == 0) {
            do {
                do {
                    std::cout << "Please enter the position of the chess piece you want to move: " ;
                    std::string start_position;
                    std::cin >> start_position;
                    start_position_index = board_position_to_index(start_position);

                    if (!chess_board[start_position_index]) {
                        std::cout << "Ummm... that's... not a chess piece...... Please try again..." << std::endl;
                    }
                } while(!chess_board[start_position_index]);
                if (chess_board[start_position_index]->get_piece_color() != this->get_piece_color()) {
                    std::cout << "You can only move a " << color_to_string(this->get_piece_color()) << " chess piece. You're not trying to cheat are you...?" << std::endl;
                }
            } while (chess_board[start_position_index]->get_piece_color() != this->get_piece_color());

            // Get the possible moves for the chosen piece
            possible_moves = chess_board[start_position_index]->get_valid_moves(start_position_index, chess_board.get_board());
            
            // For a king, must make sure that it is not being moved into check position
            if (chess_board[start_position_index]->get_symbol() == 'K') {
                std::vector<int>::iterator first_new_position{possible_moves.begin()};
                std::vector<int>::iterator last_new_position{possible_moves.end()};
                std::vector<int>::iterator possible_final_position;
                std::vector<int> moves_to_remove;
                for (possible_final_position = first_new_position ; possible_final_position < last_new_position ; ++possible_final_position) {
                    // Make each of the possible king moves on a copy of the current chess board
                    std::vector<std::shared_ptr<pcs::chess_piece>> chess_board_after_possible_next_move{chess_board.get_board()};

                    chess_board_after_possible_next_move[*possible_final_position] = std::move(chess_board_after_possible_next_move[start_position_index]);
                    chess_board_after_possible_next_move[*possible_final_position]->has_been_moved();

                    std::vector<int> opposition_possible_moves = get_all_possible_moves(opposite_color(this->get_piece_color()), chess_board_after_possible_next_move);

                    // If this possible final move for the king was found in possible opposition moves then
                    // it can be captured, therefore we will remove this from possible moves for this king
                    if (std::find(opposition_possible_moves.begin(), 
                                  opposition_possible_moves.end(),
                                  *possible_final_position) != opposition_possible_moves.end()) {
            
                        moves_to_remove.push_back(*possible_final_position);
                    }
                }  
    
                // Remove this possible position for the king, if found in opposition possible moves
                possible_moves.erase(std::remove_if(possible_moves.begin(), 
                                                    possible_moves.end(),
                                                    [&moves_to_remove](const int& position) -> bool {
                                                        return std::find(moves_to_remove.begin(),
                                                                         moves_to_remove.end(),
                                                                         position) != moves_to_remove.end();
                                                    }), possible_moves.end());
            }

            if (possible_moves.size() == 0) {
                std::cout << "Sorry, this piece has no possible moves. Please try again..." << std::endl;
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
        }
    }
    
    std::cout << "To which of the possible positions do you want to move your piece?" << std::endl;
    std::string end_position;
    std::cin >> end_position;
    int end_position_index = board_position_to_index(end_position);
    // Add check to see if correct move is entered

    return std::pair<int, int>(start_position_index, end_position_index);
}

std::pair<int, int> chess_bot::choose_move(board& chess_board) {
    std::cout << "Choose move" << std::endl;

    return std::pair<int, int>(1, 2);    
}