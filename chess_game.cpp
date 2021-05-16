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
// 10/05/2021
// - Added functionality to save game in .pgn format
// - Fixed date format when outputting .pgn file
// 15/05/2021
// - Added functionality to load a saved chess game
// - Fixed bug with recording castling moves


#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>
#include <ctime>
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
    
    // Record move
    std::stringstream move_name;
    if (type_of_move == brd::castling) {
        if (end_position_index == 6 || end_position_index == 62) {
            // Kingside castling
            move_name << "O-O";
        } else if (end_position_index == 2 || end_position_index == 58) {
            // Queenside castling
            move_name << "O-O-O";
        }
    } else {
        if (chess_board[start_position_index]->get_symbol() != 'p') {
            move_name << chess_board[start_position_index]->get_symbol();
        }
        if (chess_board[end_position_index]) {
            // Move was a capture
            move_name << "x";
        }

        move_name << brd::board_index_to_position(end_position_index);

        if (type_of_move == brd::en_passant) {
            move_name << " e.p.";
        }
    }
    moves_played.push_back(move_name.str());

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

            // Add pawn promotion to moves_played
            moves_played.back() += "=";
            moves_played.back() += chess_board[pawn_to_promote_index]->get_symbol();

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
                                               return this->chess_board[board_index]->get_symbol() == 'K' && 
                                                      this->chess_board[board_index]->get_piece_color() != this->current_player->get_piece_color();
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
        // Update record of moves
        moves_played.back() += "+" ;
    } else if (checkmate_on_opposition) {
        std::cout << "Checkmate!" << std::endl;
        status = checkmate;
        // Update record of moves
        moves_played.back() += "#" ;
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

void cgm::chess_game::save_game() {
    std::cout << "Please enter the file path where you would like the game to be save:" << std::endl;
    std::string file_save_path{};
    std::cin >> file_save_path;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::ofstream save_file(file_save_path + ".pgn");

    // Write heading of file in pgn format
    save_file << "[Event \"Pointless chess game\"]" << std::endl;
    save_file << "[Site \"Some computer somewhere\"]" << std::endl;
    std::time_t ttime;
    std::time(&ttime);
    std::tm date_and_time = *localtime(&ttime);
    int year{date_and_time.tm_year + 1900};
    int month{date_and_time.tm_mon};
    int day{date_and_time.tm_mday};
    save_file << "[Date \"" << year << "." << month << "." << day << "\" ]" << std::endl; 
    save_file << "[Round \"" << games_played << "\"]" << std::endl;
    if (players[0]->get_piece_color() == pcs::white) {
        save_file << "[White \"" << players[0]->get_name() << "\"]" << std::endl;
        save_file << "[Black \"" << players[1]->get_name() << "\"]" << std::endl;
    } else {
        save_file << "[White \"" << players[1]->get_name() << "\"]" << std::endl;
        save_file << "[Black \"" << players[0]->get_name() << "\"]" << std::endl;
    }
    if (status == checkmate) {
        if (current_player->get_piece_color() != pcs::white) {
            save_file << "[Result \"1-0\"]" << std::endl;
        } else if (current_player->get_piece_color() != pcs::black) {
            save_file << "[Result \"0-1\"]" << std::endl;
        }
    } else {
        // Game unfinished
        save_file << "[Result \"*\"]" << std::endl;
    }

    // Write game's moves to file in pgn format
    std::vector<std::string>::iterator first_move{moves_played.begin()};
    std::vector<std::string>::iterator last_move{moves_played.end()};
    std::vector<std::string>::iterator move;
    int move_number{1};
    int game_round_number{1};
    for (move = first_move ; move < last_move ; ++move) {
        if (move_number%2 != 0) {
            save_file << game_round_number << ". " << *move << " ";
        } else {
            save_file << *move << std::endl;
            ++game_round_number;
        }
        ++move_number;
    }

    if (status == checkmate) {
        if (current_player->get_piece_color() != pcs::white) {
            save_file << "1-0";
        } else if (current_player->get_piece_color() != pcs::black) {
            save_file << "0-1";
        }

    save_file.close();
    }
}

void cgm::chess_game::load_game() {
    std::cout << "Please enter the file path of the .pgn file you would like to load:" << std::endl;
    std::string file_load_path{};
    std::cin >> file_load_path;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string file_entry;
    std::vector<std::string> loaded_moves;

    std::ifstream pgn_file(file_load_path);
    if (!pgn_file.good()) {
        std::cerr << "Error: file could not be opened." << std::endl;
    }

    // Read the moves made from the pgn file
    while (!pgn_file.eof()) {
        if (!std::getline(pgn_file, file_entry)) {
            std::cerr << "Error: could not read line." << std::endl;
            pgn_file.clear();
            pgn_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            //std::getline(pgn_file, file_entry);
            if (file_entry == "[Result \"0-1\"]" || file_entry == "[Result \"1-0\"]") {
                std::cerr << "Error: this game was complete, cannot continue." << std::endl;
                break;
            } else if (file_entry.find("[") == 0) {
                continue;
            }
            std::stringstream move_played;
            move_played << file_entry;

            // Extract moves player by white and black pieces
            move_played.ignore(std::numeric_limits<std::streamsize>::max(),' ');
            std::string white_move;
            std::string black_move;
            move_played >> white_move >> black_move;

            // Add moves to their respective vectors
            if (white_move != "") {
                loaded_moves.push_back(white_move);
            }
            if (black_move != "") {
                loaded_moves.push_back(black_move);
            }
        }
    }

    // Close the file
    pgn_file.close();

    // Play the game with the loaded move to get game to the saved state
    // Get possible moves for player to check that loaded moves are valid
    std::vector<std::string>::iterator first_move{loaded_moves.begin()};
    std::vector<std::string>::iterator last_move{loaded_moves.end()};
    std::vector<std::string>::iterator move;
    for (move = first_move ; move < last_move ; ++move) {
        std::string loaded_move{*move};
        
        char piece_to_move_symbol{};
        int start_position_index{-1};
        int end_position_index{};
        bool promotion_move{false};
        brd::move_type type_of_move{brd::standard};

        if (loaded_move == "O-O"|| loaded_move == "0-0") {
            // Move is kingside castling
            type_of_move = brd::castling;
            if (current_player->get_piece_color() == pcs::white) {
                start_position_index = 4;
                end_position_index = 6;
            } else {
                start_position_index = 60;
                end_position_index = 62;
            }
        } else if (loaded_move == "O-O-O" || loaded_move == "0-0-0") {
            // Move is queenside castling
            type_of_move = brd::castling;
            if (current_player->get_piece_color() == pcs::white) {
                start_position_index = 4;
                end_position_index = 2;
            } else {
                start_position_index = 60;
                end_position_index = 58;
            }
        } else if (loaded_move.find_first_not_of("KQBNR") == 0) {
            // Move is using a pawn, get end position (and start if given) and whether
            // move is en passant or pawn get promoted
            piece_to_move_symbol = 'p';

            std::vector<char> piece_symbols{'K','Q','B','N','R'};
            if (loaded_move.find("e.p.")) {
                type_of_move = brd::en_passant;
            } else if (std::any_of(std::begin(piece_symbols), 
                                   std::end(piece_symbols),
                                   [&](const char& symbol){return loaded_move.find(symbol);})) {
                
                // Loaded move contains other piece symbol (not at the beginning)
                // therfore it a pawn promotion move
                promotion_move = true;
            }
            
            if (loaded_move.length() == 2 || loaded_move.length() == 3) {
                end_position_index = brd::board_position_to_index(loaded_move.substr(0, 1));
            } else if (loaded_move.length() == 4 || loaded_move.length() == 5) {
                start_position_index = brd::board_position_to_index(loaded_move.substr(0, 1));
                end_position_index = brd::board_position_to_index(loaded_move.substr(2, 3));
            } else {
                std::cerr << "Invalid portable game notation" << std::endl;
                exit(-1);
            }
        } else {
            piece_to_move_symbol = loaded_move[0];
            if (loaded_move.length() == 3 || loaded_move.length() == 4) {
                end_position_index = brd::board_position_to_index(loaded_move.substr(1, 2));
            } else if (loaded_move.length() == 5 || loaded_move.length() == 6) {
                start_position_index = brd::board_position_to_index(loaded_move.substr(1, 2));
                end_position_index = brd::board_position_to_index(loaded_move.substr(3, 4));
            } else {
                std::cerr << "Invalid portable game notation" << std::endl;
                exit(-1);
            }
        }
        
        bool move_valid{false};
        if (start_position_index == -1) {
            // Start position of move wasn't included in notation
            // Find start position of move
            std::vector<std::pair<int, std::vector<int>>> player_possible_moves{};
            player_possible_moves = current_player->get_player_piece_possible_moves(piece_to_move_symbol, chess_board);
            std::vector<std::pair<int, std::vector<int>>>::iterator first_possible_piece_and_moves{player_possible_moves.begin()};
            std::vector<std::pair<int, std::vector<int>>>::iterator last_possible_piece_and_moves{player_possible_moves.end()};
            std::vector<std::pair<int, std::vector<int>>>::iterator piece_and_moves;
    
            for (piece_and_moves = first_possible_piece_and_moves ;
                 piece_and_moves < last_possible_piece_and_moves ;
                 ++piece_and_moves) {

                // Find whether loaded move is possible
                std::vector<int> possible_moves = piece_and_moves->second;
                if (std::find(possible_moves.begin(), possible_moves.end(),
                    end_position_index) != possible_moves.end()) {
                    
                    move_valid = true;
                    start_position_index = piece_and_moves->first;
                    break;
                }
            }
        } else {
            // Check that move is possible from start to end position
            if (!chess_board[start_position_index] || 
                chess_board[start_position_index]->get_piece_color() != this->current_player->get_piece_color()) {
                
                std::cerr << "Invalid chess move found" << std::endl;
                exit(-1);
            } else {
                std::vector<int> piece_valid_moves{};
                piece_valid_moves = chess_board[start_position_index]->get_valid_moves(start_position_index, 
                                                                                       chess_board.get_board());
                // Find whether loaded move is possible
                bool move_valid{false};
                if (std::find(piece_valid_moves.begin(), piece_valid_moves.end(),
                    end_position_index) != piece_valid_moves.end()) {
                    
                    move_valid = true;
                }
            }
        }
        if (!move_valid) {
            std::cerr << "Invalid chess move found" << std::endl;
            exit(-1);
        }
    
        // Record move
        moves_played.push_back(loaded_move);

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

        if (promotion_move) {
            // Promote pawn
            char promotion_piece_symbol{loaded_move.back()};
            int old_pawn_id{chess_board[end_position_index]->get_id()};
            if (promotion_piece_symbol == 'N') {
                chess_board[end_position_index] = std::move(std::make_shared<pcs::knight>(pcs::knight(current_player->get_piece_color(), 
                                                                                                      old_pawn_id)));
            } else if (promotion_piece_symbol == 'B') {
                chess_board[end_position_index] = std::move(std::make_shared<pcs::bishop>(pcs::bishop(current_player->get_piece_color(), 
                                                                                                      old_pawn_id)));
            } else if (promotion_piece_symbol == 'R') {
                chess_board[end_position_index] = std::move(std::make_shared<pcs::rook>(pcs::rook(current_player->get_piece_color(),
                                                                                                  old_pawn_id)));
            } else if (promotion_piece_symbol == 'Q') {
                chess_board[end_position_index] = std::move(std::make_shared<pcs::queen>(pcs::queen(current_player->get_piece_color(),
                                                                                                    old_pawn_id)));
            }

            chess_board[end_position_index]->has_been_moved(); 
        } 

        this->update_game_status();
        this->get_next_player_ready();
    }
}
