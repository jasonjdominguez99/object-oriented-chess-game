// OOPCpp_Final_Assignment_Chess/chess_game.cpp
//
// Source file for chess game class
// 
// Author: Jason Dominguez


#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#include <tuple>
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


bool cgm::chess_game::current_player_make_a_move(bool in_check) {
    std::pair<int, int> move{};
    try {
        move = current_player->choose_move(chess_board);
    } catch (bool quit_game) {
        return quit_game;
    }
    int start_position_index = move.first;
    int end_position_index = move.second;

    brd::move_type type_of_move{brd::standard};
    // Must be en passant move if pawn is moving diagonally into empty square
    if (chess_board[start_position_index]->get_symbol() == 'p' && !chess_board[end_position_index]) {
        if (end_position_index == start_position_index + 8 + 1 || end_position_index == start_position_index + 8 - 1 || 
            end_position_index == start_position_index - 8 + 1 || end_position_index == start_position_index - 8 - 1) 
        {
            type_of_move = brd::en_passant;
        }
    } else if (chess_board[start_position_index]->get_symbol() == 'K' && !chess_board[end_position_index]) {
        if (end_position_index == start_position_index + 2 || end_position_index == start_position_index - 2){
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
        // For moves that can be made by multiple identical pieces from
        // different starting positions (ambiguous), record starting position
        std::vector<int> other_pieces_possible_moves{};
        for (int i{} ; i < 8*8 ; i++) {
            if (chess_board[i] && chess_board[i]->get_piece_color() == current_player->get_piece_color() && 
                chess_board[i]->get_symbol() == chess_board[start_position_index]->get_symbol() &&
                i != start_position_index) 
            {
                std::vector<int> possible_final_positions;
                possible_final_positions = chess_board[i]->get_valid_moves(i, std::move(chess_board.get_board()));
                if (possible_final_positions.size() == 0) {
                    continue;
                } else {
                    other_pieces_possible_moves.insert(std::end(other_pieces_possible_moves), 
                                                       std::begin(possible_final_positions),
                                                       std::end(possible_final_positions));
                }
            }
        }
        // Find if any other moves by the same piece get to the same end position
        bool move_is_ambiguous = std::find_if(other_pieces_possible_moves.begin(), 
                                              other_pieces_possible_moves.end(),
                                              [end_position_index](int board_index) {
                                                  return board_index == end_position_index;
                                              }) != other_pieces_possible_moves.end();
        if (move_is_ambiguous) {
            move_name << brd::board_index_to_position(start_position_index);
        }
        if (chess_board[end_position_index]) {
            // Move was a capture
            move_name << "x";
        }

        move_name << brd::board_index_to_position(end_position_index);

        if (type_of_move == brd::en_passant) {
            move_name << "e.p.";
        }
    }

    // Add move to moves played (for saving)
    moves_played.push_back(move_name.str());

    chess_board.move_piece(start_position_index, end_position_index, type_of_move);

    // Reset all pawns to have no possibility of being captured via en passant
    for (int i{} ; i < 8*8 ; i++) {
        if (chess_board[i] && chess_board[i]->get_symbol() == 'p') {
            chess_board[i]->set_en_passant_possibility(false);
        }
    }

    // If pawn was just moved by two and it was its first move, it can be captured via en passant
    if (chess_board[end_position_index]->get_symbol() == 'p' && chess_board[end_position_index]->get_has_moved() == false) {
        int pawn_position_row{end_position_index/8};
        int fourth_row_index{3};
        int fifth_row_index{4};
        if (chess_board[end_position_index]->get_piece_color() == pcs::white && pawn_position_row == fourth_row_index) {
            chess_board[end_position_index]->set_en_passant_possibility(true);
        } else if (chess_board[end_position_index]->get_piece_color() == pcs::black && pawn_position_row == fifth_row_index) {
            chess_board[end_position_index]->set_en_passant_possibility(true);
        }
    }

    // Setting has_been_moved must be after checking for en passant possibility as only pawns whose first move 
    // was two steps can be captured en passant, so need to know if pawn had moved before this move
    chess_board[end_position_index]->has_been_moved();

    // For castling, rook has also been moved
    if (type_of_move == brd::castling) {
        if (end_position_index > start_position_index) {
            chess_board[end_position_index - 1]->has_been_moved();
        } else {
            chess_board[end_position_index + 1]->has_been_moved();
        }
    }
    return false;
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

            // Get user input for which piece to which they want to promote their pawn
            while (true) {
                std::cout << "Please choose which of these pieces to which you want to promote your pawn: (N, B, R, Q)" << std::endl;
                std::string chosen_promotion_piece_symbol;
                std::cin >> chosen_promotion_piece_symbol;

                bool has_moved{true};
                if (chosen_promotion_piece_symbol == "N") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_unique<pcs::knight>(pcs::knight(
                                                                                                     current_player->get_piece_color(), 
                                                                                                     old_pawn_id,
                                                                                                     has_moved)));
                    break;
                } else if (chosen_promotion_piece_symbol == "B") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_unique<pcs::bishop>(pcs::bishop(
                                                                                                     current_player->get_piece_color(),
                                                                                                     old_pawn_id,
                                                                                                     has_moved)));
                    break;
                } else if (chosen_promotion_piece_symbol == "R") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_unique<pcs::rook>(pcs::rook(
                                                                                                   current_player->get_piece_color(),
                                                                                                   old_pawn_id,
                                                                                                   has_moved)));
                    break;
                } else if (chosen_promotion_piece_symbol == "Q") {
                    chess_board[pawn_to_promote_index] = std::move(std::make_unique<pcs::queen>(pcs::queen(
                                                                                                    current_player->get_piece_color(),
                                                                                                    old_pawn_id,
                                                                                                    has_moved)));
                    break;
                } else {
                    std::cout << "You MUST promote to one of these pieces (N, B, R, Q)" << std::endl;
                    std::cout << "Let's try this again..." << std::endl;
                }
            }

            std::cout << std::endl << this->get_chess_board() << std::endl;

            // Add pawn promotion to moves_played (for saving)
            moves_played.back() += "=";
            moves_played.back() += chess_board[pawn_to_promote_index]->get_symbol();

            break;
        }
    }
}


void cgm::chess_game::update_game_status() {
    std::vector<int> possible_next_moves{};

    // See if check has occured (current player can capture the opposition king on their next turn)
    possible_next_moves = std::get<2>(pcs::get_all_possible_moves(current_player->get_piece_color(), 
                                                                  std::move(chess_board.get_board()) ));
    // Find if any of these moves will capture the opposition's king
    bool check_on_opposition{false};
    check_on_opposition = std::find_if(possible_next_moves.begin(), possible_next_moves.end(),
                                       [this](int board_index) {
                                           return this->chess_board[board_index] &&
                                                  this->chess_board[board_index]->get_symbol() == 'K' && 
                                                  this->chess_board[board_index]->get_piece_color() != current_player->get_piece_color();
                                       }) != possible_next_moves.end();
    
    // See if checkmate has occured (this player will capture the opposition king on their next turn)
    // To avoid checkmate, opposition must capture threatening piece, block the king, or move king out of check
    bool checkmate_on_opposition{};
    std::vector<bool> checkmate_ocurrences{};
    if (check_on_opposition) {
        std::tuple<std::vector<int>, std::vector<std::vector<int>>, std::vector<int>> opposition_possible_next_moves_and_start_positions{};
        opposition_possible_next_moves_and_start_positions = pcs::get_all_possible_moves(
                                                                      pcs::opposite_color(current_player->get_piece_color()),
                                                                      std::move(chess_board.get_board()) );
        
        std::vector<int> opposition_start_positions{std::get<0>(opposition_possible_next_moves_and_start_positions)};
        std::vector<std::vector<int>> opposition_possible_next_moves{std::get<1>(opposition_possible_next_moves_and_start_positions)};
        
        std::vector<int>::iterator first_start_position{opposition_start_positions.begin()};
        std::vector<int>::iterator last_start_position{opposition_start_positions.end()};
        std::vector<int>::iterator possible_start_position;
        int i{};
        for (possible_start_position = first_start_position ; possible_start_position < last_start_position ; ++possible_start_position) {
            std::vector<int>::iterator first_move{opposition_possible_next_moves[i].begin()};
            std::vector<int>::iterator last_move{opposition_possible_next_moves[i].end()};
            std::vector<int>::iterator possible_move;
            for (possible_move = first_move ; possible_move < last_move ; ++possible_move) {
                // Temporarily make each opposition move and see if king can be saved
                std::vector<std::unique_ptr<pcs::chess_piece>> chess_board_after_possible_move{std::move(chess_board.get_board())};
                
                /*
                // Will have to edit this for en passant and castling moves
                chess_board_after_possible_move[*possible_move] = std::move(chess_board_after_possible_move[*possible_start_position]);
                chess_board_after_possible_move[*possible_move]->has_been_moved();
                */
                if (chess_board_after_possible_move[*possible_start_position]->get_symbol() == 'p' &&  
                    !chess_board_after_possible_move[*possible_move] &&
                    (*possible_move == *possible_start_position + 8 + 1 || *possible_move == *possible_start_position + 8 - 1) ) {

                    // En passant move upwards
                    chess_board[*possible_move] = std::move(chess_board[*possible_move - 8]);
                } else if (chess_board_after_possible_move[*possible_start_position]->get_symbol() == 'p' &&  
                           !chess_board_after_possible_move[*possible_move] &&
                           (*possible_move == *possible_start_position - 8 + 1 || *possible_move == *possible_start_position - 8 - 1) ) {
                
                    // En passant move down
                    chess_board[*possible_move] = std::move(chess_board[*possible_move + 8]);
                }
                chess_board_after_possible_move[*possible_move] = std::move(chess_board_after_possible_move[*possible_start_position]);
                chess_board_after_possible_move[*possible_move]->has_been_moved(); 

                // Deep copy the chess board
                std::vector<std::unique_ptr<pcs::chess_piece>> board_after_possible_move_copy{};
                for (size_t i{0} ; i < 8*8 ; i++) {
                    if (!chess_board_after_possible_move[i]) {
                        board_after_possible_move_copy.push_back(std::unique_ptr<pcs::chess_piece>{nullptr});
                    } else {
                        board_after_possible_move_copy.push_back(chess_board_after_possible_move[i]->clone());
                    }
                }

                std::vector<int> future_current_player_possible_moves = std::get<2>(pcs::get_all_possible_moves(
                                                                                             current_player->get_piece_color(), 
                                                                                             std::move(chess_board_after_possible_move) ));
                // Find if any of these moves will capture the opposition's king
                bool king_can_be_captured = std::find_if(future_current_player_possible_moves.begin(), 
                                                         future_current_player_possible_moves.end(),
                                                         [&board_after_possible_move_copy, this](int board_index) {
                                                             return board_after_possible_move_copy[board_index] &&
                                                                    board_after_possible_move_copy[board_index]->get_symbol() == 'K' && 
                                                                    (board_after_possible_move_copy[board_index]->get_piece_color() != 
                                                                     this->current_player->get_piece_color() );
                                                         }) != future_current_player_possible_moves.end();
 
                if (king_can_be_captured) {
                    checkmate_ocurrences.push_back(true);
                } else {
                    checkmate_ocurrences.push_back(false);
                    break;
                }
            }
            checkmate_on_opposition = std::all_of(checkmate_ocurrences.begin(), checkmate_ocurrences.end(),
                                                  [] (bool checkmate_occurrence) { return checkmate_occurrence; });
            
            if (!checkmate_on_opposition) { break; }
            ++i;
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
    } else if (this->status == active) {
        while (true) {
            std::cout << "Do you want to count that game as a draw (y/n)?" << std::endl;
            std::string input;
            std::cin >> input;
            if (input == "y") {
                std::cout << std::endl << "Draw!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ++draws;
                status = draw;
                break;
            } else if (input == "n") {
                std::cout << std::endl << "Game unfinished" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            } else {
                std::cout << "" << std::endl;
                std::cout << "Let's try this again..." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
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
        std::cout << "Overall a draw... pretty boring..." << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
}


void cgm::chess_game::save_game() {
    std::cout << "Please enter a name for the saved file:" << std::endl;
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
    } else if (status == draw) {
        save_file << "[Result \"1/2-1/2\"]" << std::endl;
    } else {
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
    
    // Write the final score (if game finished) file
    if (status == checkmate) {
        if (current_player->get_piece_color() != pcs::white) {
            save_file << "1-0";
        } else if (current_player->get_piece_color() != pcs::black) {
            save_file << "0-1";
        }
    } else if (status == draw) {
        save_file << "1/2-1/2";
    }
    save_file.close();
}


namespace cgm {
    std::vector<std::string> read_pgn(std::string file_name) {
        std::string file_entry;
        std::vector<std::string> loaded_moves;

        std::ifstream pgn_file{file_name};
        pgn_file.exceptions(std::ifstream::badbit);

        // Read the moves made from the pgn file
        while (pgn_file.good()) {
            std::getline(pgn_file, file_entry);
            if (file_entry.find("[") == 0) {
                // Don't require the header lines
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
            if (white_move != "" && white_move != "1-0" && white_move != "1/2-0" &&
                white_move != "0-1/2" && white_move != "0-1") 
            {
                loaded_moves.push_back(white_move);
            }
            if (black_move != "" && black_move != "1-0" && black_move != "1/2-0" &&
                black_move != "0-1/2" && black_move != "0-1")
            {
                loaded_moves.push_back(black_move);
            }
        }
        // Close the file
        pgn_file.close();

        return loaded_moves;
    }


    std::tuple<char, int, int, bool, char, brd::move_type> parse_pgn_move_notation(std::string loaded_move, pcs::color player_color) {
        char piece_to_move_symbol{};
        int start_position_index{-1};
        int end_position_index{};
        bool promotion_move{false};
        char promotion_piece{};
        brd::move_type type_of_move{brd::standard};
        
        // Remove check, capture or checkmate notation
        // as it is not needed to recreate the move
        if (loaded_move[loaded_move.length() - 1] == '+' || loaded_move[loaded_move.length() - 1] == '#') {
            loaded_move.erase(loaded_move.begin() + loaded_move.length() - 1);
        }
        size_t x_pos{loaded_move.find('x')};
        if (x_pos != std::string::npos) {
            loaded_move.erase(loaded_move.begin() + x_pos);
        }

        if (loaded_move == "O-O"|| loaded_move == "0-0") {
            // Move is kingside castling
            type_of_move = brd::castling;
            if (player_color == pcs::white) {
                start_position_index = 4;
                end_position_index = 6;
            } else {
                start_position_index = 60;
                end_position_index = 62;
            }
        } else if (loaded_move == "O-O-O" || loaded_move == "0-0-0") {
            // Move is queenside castling
            type_of_move = brd::castling;
            if (player_color == pcs::white) {
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
            std::string en_passant_notation{"e.p."};
            size_t en_passant_pos = loaded_move.find(en_passant_notation);
            
            if (en_passant_pos != std::string::npos) {
                type_of_move = brd::en_passant;
                // Remove en passant notation from move
                loaded_move.erase(en_passant_pos, en_passant_notation.length());
            } else if (std::any_of(std::begin(piece_symbols), std::end(piece_symbols), 
                                       [&](const char& symbol) {
                                           return loaded_move.find(symbol) != std::string::npos;
                                       })) {
                
                // Loaded move contains other piece symbol (not at the beginning)
                // therefore it a pawn promotion move
                promotion_move = true;
                // Remove promotion notation from move and get promotion piece symbol
                promotion_piece = loaded_move[loaded_move.length() - 1];
                loaded_move.erase(loaded_move.length() - 2, 2);
            }
            
            if (loaded_move.length() == 2) {
                end_position_index = brd::board_position_to_index(loaded_move);
            } else if (loaded_move.length() == 4) {
                start_position_index = brd::board_position_to_index(loaded_move.substr(0, 2));
                end_position_index = brd::board_position_to_index(loaded_move.substr(2, 2));
            } else {
                throw "Invalid portable game notation";
            }
        } else {
            piece_to_move_symbol = loaded_move[0];
            
            if (loaded_move.length() == 3) {
                end_position_index = brd::board_position_to_index(loaded_move.substr(1, 2));
            } else if (loaded_move.length() == 5) {
                start_position_index = brd::board_position_to_index(loaded_move.substr(1, 2));
                end_position_index = brd::board_position_to_index(loaded_move.substr(3, 2));
            } else {
                throw "Invalid portable game notation";
            }
        }

        return std::make_tuple(piece_to_move_symbol, start_position_index, end_position_index,
                               promotion_move, promotion_piece, type_of_move);
    }
}


void cgm::chess_game::load_game() {
    // Get the file to be loaded in, handling any exceptions for bad file names
    while (true) {
        try {
            std::cout << "Please enter the name of the .pgn file you would like to load:" << std::endl;
            std::string file_name{};
            std::cin >> file_name;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
            std::vector<std::string> loaded_moves;
            loaded_moves = read_pgn(file_name);

            std::cout << std::endl << "Loading game..." << std::endl << std::endl;

            // Ensure board is in initial detup before loading moves
            chess_board.reset();

            // Play the game with the loaded move to get game to the saved state
            // Get possible moves for player to check that loaded moves are valid
            std::vector<std::string>::iterator first_move{loaded_moves.begin()};
            std::vector<std::string>::iterator last_move{loaded_moves.end()};
            std::vector<std::string>::iterator move;
            for (move = first_move ; move < last_move ; ++move) {
                std::string loaded_move{*move};
                std::string unedited_loaded_move{loaded_move}; // deep copy to be added to moves_played later

                // Get move information from the pgn notation
                std::tuple<char, int, int, 
                           bool, char, brd::move_type> move_info = parse_pgn_move_notation(loaded_move, current_player->get_piece_color());

                char piece_to_move_symbol{std::get<0>(move_info)};
                int start_position_index{std::get<1>(move_info)};
                int end_position_index{std::get<2>(move_info)};
                bool promotion_move{std::get<3>(move_info)};
                char promotion_piece{std::get<4>(move_info)};
                brd::move_type type_of_move{std::get<5>(move_info)};

                // Find whether move from file is valid
                bool move_valid{false};
                if (start_position_index == -1) {
                    // Start position of move wasn't included in notation
                    // Find start position of move
                    std::vector<std::pair<int, std::vector<int>>> player_possible_moves{};
                    brd::board copy_of_board{chess_board};
                    player_possible_moves = current_player->get_player_piece_possible_moves(piece_to_move_symbol, 
                                                                                            std::move(copy_of_board) );
                    std::vector<std::pair<int, std::vector<int>>>::iterator first_possible_piece_and_moves{player_possible_moves.begin()};
                    std::vector<std::pair<int, std::vector<int>>>::iterator last_possible_piece_and_moves{player_possible_moves.end()};
                    std::vector<std::pair<int, std::vector<int>>>::iterator piece_and_moves;
    
                    for (piece_and_moves = first_possible_piece_and_moves ;
                         piece_and_moves < last_possible_piece_and_moves ;
                         ++piece_and_moves) 
                    {
                        // Find whether loaded move is possible
                        std::vector<int> possible_moves = piece_and_moves->second;
                        if (std::find(possible_moves.begin(), possible_moves.end(), end_position_index) != possible_moves.end()) {
                            move_valid = true;
                            start_position_index = piece_and_moves->first;
                            break;
                        }
                    }
                } else {
                    // Check that move is possible from start to end position
                    if (!chess_board[start_position_index] || 
                        chess_board[start_position_index]->get_piece_color() != this->current_player->get_piece_color()) 
                    {   
                        throw "Invalid chess move found";
                    } else {
                        std::vector<int> piece_valid_moves{};
                        piece_valid_moves = chess_board[start_position_index]->get_valid_moves(start_position_index, 
                                                                                               std::move(chess_board.get_board()));
                        // Find whether loaded move is possible
                        if (std::find(piece_valid_moves.begin(), piece_valid_moves.end(), end_position_index) != piece_valid_moves.end()) {
                            move_valid = true;
                        }
                    }
                }
                if (!move_valid) {
                    throw "Invalid chess move found";
                }
    
                // Record move
                moves_played.push_back(unedited_loaded_move);

                chess_board.move_piece(start_position_index, end_position_index, type_of_move);

                // Reset all pawns to have no possibility of being captured via en passant
                for (int i{} ; i < 8*8 ; i++) {
                    if (chess_board[i] && chess_board[i]->get_symbol() == 'p') {
                        chess_board[i]->set_en_passant_possibility(false);
                    }
                }
                // If pawn was just moved by two and it was its first move, 
                // it can be captured via en passant
                if (chess_board[end_position_index]->get_symbol() == 'p' && chess_board[end_position_index]->get_has_moved() == false) {
                    int pawn_position_row{end_position_index/8};
                    int fourth_row_index{3};
                    int fifth_row_index{4};
                    if (chess_board[end_position_index]->get_piece_color() == pcs::white && pawn_position_row == fourth_row_index) {
                        chess_board[end_position_index]->set_en_passant_possibility(true);
                    } else if (chess_board[end_position_index]->get_piece_color() == pcs::black && pawn_position_row == fifth_row_index) {
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
                    int old_pawn_id{chess_board[end_position_index]->get_id()};
                    if (promotion_piece == 'N') {
                        chess_board[end_position_index] = std::move(std::make_unique<pcs::knight>(pcs::knight(
                                                                                                      current_player->get_piece_color(), 
                                                                                                      old_pawn_id,
                                                                                                      true) ));
                    } else if (promotion_piece == 'B') {
                        chess_board[end_position_index] = std::move(std::make_unique<pcs::bishop>(pcs::bishop(
                                                                                                      current_player->get_piece_color(), 
                                                                                                      old_pawn_id,
                                                                                                      true) ));
                    } else if (promotion_piece == 'R') {
                        chess_board[end_position_index] = std::move(std::make_unique<pcs::rook>(pcs::rook(
                                                                                                    current_player->get_piece_color(),
                                                                                                    old_pawn_id,
                                                                                                    true) ));
                    } else if (promotion_piece == 'Q') {
                        chess_board[end_position_index] = std::move(std::make_unique<pcs::queen>(pcs::queen(
                                                                                                     current_player->get_piece_color(),
                                                                                                     old_pawn_id,
                                                                                                     true) ));
                    }
                } 
                this->update_game_status();
                this->get_next_player_ready();
            }
            // Chosen file successfully loaded in, get out of loop
            break;
        } catch (const std::ifstream::failure& e) {
            std::cerr << std::endl << "Error: file could not be opened." << std::endl;
            std::cout << "Please try again with a valid file name" << std::endl << std::endl;
        } catch (const char* error) {
            std::cerr << std::endl << "Error: " << error << std::endl;
            std::cout << "Please try again with a valid .pgn file" << std::endl << std::endl;
        }
    }
}