// OOPCpp_Final_Assignment_Chess/player_class.cpp
//
// Source file for player class
// 
// Author: Jason Dominguez


#include <algorithm>
#include <vector>
#include <tuple>
#include <iterator>
#include <utility>
#include <random>
#include <chrono>
#include "player_class.hpp"
#include "chess_pieces.hpp"
#include "chess_board.hpp"


// Class and function definitions
void plr::player::ask_for_name() {
    std::cout << "Please enter your name: ";
    std::string player_name{};
    std::getline(std::cin, player_name);
    while (true) {
        if (std::cin.fail() || player_name == "") {
            std::cin.clear();
            std::cout << "Surely you must have a name..." << std::endl;
            std::cout << "Please enter your name: ";
            std::getline(std::cin, player_name);
        } else if (player_name.length() > 20) {
            std::cin.clear();
            std::cout << "That's a bit of a mouthful, how about we try again "
                         "and find you a nickname less than 20 characters long?" << std::endl;
            std::cout << "Please enter your name: ";
            std::getline(std::cin, player_name);
        } else {
            break;
        }
    } 
    name = player_name;
}


void plr::player::ask_for_color() {
    std::cout << name << " please enter your chess piece color (white/black): ";
    std::string chess_piece_color;
    std::getline(std::cin, chess_piece_color);
    while (std::cin.fail() || (chess_piece_color != "white" && chess_piece_color != "black")) {
        std::cin.clear();
        std::cout << "Surely you know that chess pieces are only white or black...? Let's try again..." << std::endl;
        std::cout << name << " please enter your chess piece color (white/black): ";
        std::getline(std::cin, chess_piece_color);
    }
    piece_color = pcs::string_to_color(chess_piece_color);
}


std::pair<int, int> plr::human_player::choose_move(const brd::board& chess_board) {
    std::cout << name << "'s turn..." << std::endl;

    bool quit_game{false};
    int start_position_index{};
    std::vector<int> possible_final_positions{};
    std::pair<int, std::vector<int>> possible_moves{};
    std::vector<std::string> possible_moves_board_positions{};
    // Make sure player chooses one of their color pieces which has possible moves and not a blank chess board tile
    while (true) {
        possible_final_positions.clear();
        possible_moves_board_positions.clear();
        while (possible_final_positions.size() == 0) {
            do {
                do {
                    std::cout << "Please enter the position of the chess piece you want to move (Press 'q' to quit): " ;
                    std::string start_position;
                    std::getline(std::cin, start_position);

                    if (start_position == "q") {
                        quit_game = true;
                        throw quit_game;
                    }

                    while (true) {
                        try {
                            start_position_index = brd::board_position_to_index(start_position);
                            break;
                        } catch (const std::out_of_range& e) {
                            std::cin.clear();
                            std::cout << "Please enter a valid board position: ";
                            std::getline(std::cin, start_position);
                        }
                    }

                    if (!chess_board[start_position_index]) {
                        std::cout << "Ummm... that's... not a chess piece...... Please try again..." << std::endl;
                    }
                } while (!chess_board[start_position_index]);
                if (chess_board[start_position_index]->get_piece_color() != this->get_piece_color()) {
                    std::cout << "You can only move a " << color_to_string(this->get_piece_color()) 
                              << " chess piece. You're not trying to cheat are you...?" << std::endl;
                }
            } while (chess_board[start_position_index]->get_piece_color() != this->get_piece_color());

            // Get the possible moves for the chosen piece
            possible_final_positions = chess_board[start_position_index]->get_valid_moves(start_position_index, std::move(chess_board.get_board()));
            
            // Account for not moving into check
            brd::board chess_board_copy{chess_board};
            possible_moves = this->get_piece_valid_moves(start_position_index, possible_final_positions, std::move(chess_board_copy));

            if (possible_moves.second.size() == 0) {
                std::cout << "Sorry, this piece has no possible moves. Please try again..." << std::endl;
            }
        }

        std::cout << "Here are the possible moves for this chess piece: " << std::endl;
        std::vector<int>::iterator vector_begin{possible_moves.second.begin()};
        std::vector<int>::iterator vector_end{possible_moves.second.end()};
        std::vector<int>::iterator vector_iterator;
        for (vector_iterator = vector_begin ; vector_iterator < vector_end ; ++vector_iterator) {
            std::string board_position{brd::board_index_to_position(*vector_iterator)};
            std::cout << board_position << std::endl;
            possible_moves_board_positions.push_back(board_position);
        }
    
        std::cout << "Do you still want to move this chess piece (y/n)?" << std::endl;
        std::string piece_to_move_decision{};
        std::getline(std::cin, piece_to_move_decision);
        while (piece_to_move_decision != "y" && piece_to_move_decision != "n") {
            std::cin.clear();
            std::cout << "Please enter y or n: ";
            std::getline(std::cin, piece_to_move_decision);
        }
        if (piece_to_move_decision == "y") {
            break;
        }
    }
    
    std::cout << "To which of these positions do you want to move your piece?" << std::endl;
    std::string end_position{};
    std::getline(std::cin, end_position);
    while (std::cin.fail() || std::find(possible_moves_board_positions.begin(), possible_moves_board_positions.end(),
                                        end_position) == possible_moves_board_positions.end()) {
        
        std::cin.clear();
        std::cout << "That's not a legal move... Do you even know how to play chess?!" << std::endl;
        std::cout << "Please enter one of the possible positions to which this piece can move: ";
        std::getline(std::cin, end_position);
    }
    int end_position_index = brd::board_position_to_index(end_position);

    return std::pair<int, int>(start_position_index, end_position_index);
}


std::pair<int, int> plr::chess_bot::choose_move(const brd::board& chess_board) {
    std::cout << "ChessBot's turn..." << std::endl;
    std::cout << "Choosing move..." << std::endl;

    // Randomly choose one of their color pieces which has 
    // possible moves and not a blank chess board tile
    std::vector<std::pair<int, std::vector<int>>> all_possible_moves{};
    brd::board copy_of_board{chess_board};
    
    all_possible_moves = this->get_player_possible_moves(copy_of_board);

    // Select random piece to move, which has valid moves
    std::default_random_engine engine;
    engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distr(0, all_possible_moves.size() - 1);
    int random_start_position_index = distr(engine);

    int chosen_start_position = all_possible_moves[random_start_position_index].first;

    // Select random move from piece's possible moves
    std::default_random_engine engine2;
    engine2.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distr2(0, all_possible_moves[random_start_position_index].second.size() - 1);
    int random_final_position_index = distr2(engine2);
    
    int chosen_final_position = all_possible_moves[random_start_position_index].second[random_final_position_index];

    return std::pair<int, int>(chosen_start_position, chosen_final_position);
}


std::vector<std::pair<int, std::vector<int>>> plr::player::get_player_possible_moves(const brd::board& original_chess_board) {
    // Get all possible moves for all player's pieces
    std::vector<std::pair<int, std::vector<int>>> all_possible_moves{};   
    for (int i{} ; i < 8*8 ; i++) {
        // Deep copy original chess board to make temporary moves testing validity
        brd::board chess_board{original_chess_board};
        if (chess_board[i] && chess_board[i]->get_piece_color() == this->piece_color) {
            std::vector<int> possible_final_positions;
            possible_final_positions = chess_board[i]->get_valid_moves(i, std::move(chess_board.get_board()));

            if (possible_final_positions.size() == 0) {
                continue;
            }

            std::pair<int, std::vector<int>> possible_moves = this->get_piece_valid_moves(i, possible_final_positions, 
                                                                                          std::move(chess_board));
            
            // Account for pieces with no possible moves (i.e. king had moves removed and now has none)
            if (possible_moves.second.size() > 0) {
                all_possible_moves.push_back(possible_moves);
            }
        }
    }
    return all_possible_moves;
}


std::vector<std::pair<int, std::vector<int>>> plr::player::get_player_piece_possible_moves(char chess_piece, 
                                                                                           const brd::board& original_chess_board) {
    // Get all possible moves for all player's specified piece
    std::vector<std::pair<int, std::vector<int>>> all_possible_moves{};
    
    // Deep copy original chess board to make temporary moves testing validity
    brd::board chess_board{original_chess_board};
    
    for (int i{} ; i < 8*8 ; i++) {
        if (chess_board[i] && chess_board[i]->get_piece_color() == this->piece_color && chess_board[i]->get_symbol() == chess_piece) {
            std::vector<int> possible_final_positions;
            possible_final_positions = chess_board[i]->get_valid_moves(i, std::move(chess_board.get_board()));

            if (possible_final_positions.size() == 0) {
                continue;
            }

            std::pair<int, std::vector<int>> possible_moves = this->get_piece_valid_moves(i, possible_final_positions, 
                                                                                          std::move(chess_board));

            // Account for pieces with no possible moves (i.e. king had moves removed and now has none)
            if (possible_moves.second.size() > 0) {
                all_possible_moves.push_back(possible_moves);
            }
        }
    }
    return all_possible_moves;
}


std::pair<int, std::vector<int>> plr::player::get_piece_valid_moves(int start_position_index, std::vector<int> possible_final_positions,
                                                                    const brd::board& chess_board) {
    // For a king, must make sure that it is not being moved into check position
    if (chess_board[start_position_index]->get_symbol() == 'K') {
        // Also need to make sure castling won't be used to move
        // though positions which the opposition can attack so temporarily add these positions
        // to vector to see if opposition could attack them
        bool castle_check_position_one{false};
        if (std::find(possible_final_positions.begin(), possible_final_positions.end(),
                      start_position_index + 2) != possible_final_positions.end()) {
                    
            possible_final_positions.push_back(start_position_index + 1);
            castle_check_position_one = true;
        }
        bool castle_check_position_two{false};
        if (std::find(possible_final_positions.begin(), possible_final_positions.end(),
                      start_position_index - 2) != possible_final_positions.end()) {
                    
            possible_final_positions.push_back(start_position_index - 1);
            castle_check_position_two = true;
        }
                
        std::vector<int>::iterator first_new_position{possible_final_positions.begin()};
        std::vector<int>::iterator last_new_position{possible_final_positions.end()};
        std::vector<int>::iterator possible_final_position;
        std::vector<int> moves_to_remove;
        for (possible_final_position = first_new_position ; possible_final_position < last_new_position ; ++possible_final_position) {
            // Make each of the possible king moves on a copy of the current chess board
            std::vector<std::unique_ptr<pcs::chess_piece>> chess_board_after_possible_move{std::move(chess_board.get_board())};

            brd::board board_after_possible_move{chess_board_after_possible_move};
            board_after_possible_move.move_piece(start_position_index, *possible_final_position);

            std::vector<int> opposition_possible_moves = std::get<2>(pcs::get_all_possible_moves(
                                                                              pcs::opposite_color(this->get_piece_color()), 
                                                                              std::move(board_after_possible_move.get_board())));

            // If this possible final move for the king was found in possible opposition moves then
            // it can be captured, therefore we will remove this from possible moves for this king
            if (std::find(opposition_possible_moves.begin(), opposition_possible_moves.end(),
                          *possible_final_position) != opposition_possible_moves.end()) {
                
                moves_to_remove.push_back(*possible_final_position);
            }
        }  

        // Remove the moves added to check if castling was possible from possible final positions and remove castling move from
        // possible final positions if they are in moves to remove (i.e. they would put the king in check)
        if (castle_check_position_two) {
            std::vector<int>::iterator position = std::find(possible_final_positions.begin(), possible_final_positions.end(), 
                                                            start_position_index - 1);
            if (position != possible_final_positions.end()) {
                possible_final_positions.erase(position);
            }
            if (std::find(moves_to_remove.begin(), moves_to_remove.end(), start_position_index - 1) != moves_to_remove.end()) {
                possible_final_positions.erase(std::remove(possible_final_positions.begin(), possible_final_positions.end(), 
                                                           start_position_index - 2), possible_final_positions.end());    
            }
        }
        if (castle_check_position_one) {
            std::vector<int>::iterator position = std::find(possible_final_positions.begin(), possible_final_positions.end(), 
                                                            start_position_index + 1);
            if (position != possible_final_positions.end()) {
                possible_final_positions.erase(position);
            }
            if (std::find(moves_to_remove.begin(), moves_to_remove.end(), start_position_index + 1) != moves_to_remove.end()) {
                possible_final_positions.erase(std::remove(possible_final_positions.begin(), possible_final_positions.end(), 
                                                           start_position_index + 2), possible_final_positions.end());    
            }
        }
        // Remove possible positions for the king, if found in opposition possible moves
        possible_final_positions.erase(std::remove_if(possible_final_positions.begin(), possible_final_positions.end(),
                                                      [&moves_to_remove](const int& position) -> bool {
                                                           return std::find(moves_to_remove.begin(),
                                                                            moves_to_remove.end(),
                                                                            position) != moves_to_remove.end();
                                                       }), possible_final_positions.end());
    } else {
        // For pieces other than king, make sure they don't leave the king in a checkmate position
        // Get current player king position
        int king_position_index{};
        for (int i{} ; i < 8*8 ; i++) {
            if (chess_board[i] && chess_board[i]->get_piece_color() == this->get_piece_color() && chess_board[i]->get_symbol() == 'K') {
                king_position_index = i;
            }
        }

        // See what board layout will be for all possible moves and make sure king is not able to be attacked
        std::vector<int>::iterator first_new_position{possible_final_positions.begin()};
        std::vector<int>::iterator last_new_position{possible_final_positions.end()};
        std::vector<int>::iterator possible_final_position;
        std::vector<int> moves_to_remove;
        for (possible_final_position = first_new_position ; possible_final_position < last_new_position ; ++possible_final_position) {
            // Make each of the possible moves on a copy of the current chess board
            std::vector<std::unique_ptr<pcs::chess_piece>> chess_board_after_possible_move{std::move(chess_board.get_board())};

            brd::board board_after_possible_move{chess_board_after_possible_move};
            board_after_possible_move.move_piece(start_position_index, *possible_final_position);
           
            std::vector<int> opposition_possible_moves = std::get<2>(pcs::get_all_possible_moves(
                                                                              pcs::opposite_color(this->get_piece_color()), 
                                                                              std::move(board_after_possible_move.get_board())));
            // If the king was found in possible opposition moves then
            // it can be captured, therefore we will remove this from possible moves for this king
            if (std::find(opposition_possible_moves.begin(), opposition_possible_moves.end(),
                          king_position_index) != opposition_possible_moves.end()) 
            {
                moves_to_remove.push_back(*possible_final_position);
            }
        }  
        // Remove this possible position
        possible_final_positions.erase(std::remove_if(possible_final_positions.begin(), possible_final_positions.end(),
                                                      [&moves_to_remove](const int& position) -> bool {
                                                              return std::find(moves_to_remove.begin(),
                                                                               moves_to_remove.end(),
                                                                               position) != moves_to_remove.end();
                                                       }), possible_final_positions.end());
    }  
    std::pair<int, std::vector<int>> possible_moves = std::pair<int, std::vector<int>>(start_position_index, possible_final_positions);
    
    return possible_moves;
}