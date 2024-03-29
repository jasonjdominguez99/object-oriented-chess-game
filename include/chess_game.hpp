// object-oriented-chess-game/chess_game.cpp
//
// Header file for chess game class
// 
// Author: Jason Dominguez


#ifndef CHESS_GAME_H
#define CHESS_GAME_H


#include <string>
#include <tuple>
#include "chess_board.hpp"


// Forward declarations
namespace plr {
    class player;
}

namespace brd {
    class board;
}


// Class and function definitions
namespace cgm {
    enum game_status {active, 
                      check,
                      checkmate,
                      draw};
    
    std::vector<std::string> read_pgn(std::string file_name);
    std::tuple<char, int, int, bool, char, brd::move_type> parse_pgn_move_notation(std::string loaded_move, pcs::color player_color);

    class chess_game
    {
    private:
        std::vector<std::shared_ptr<plr::player>> players;
        std::shared_ptr<plr::player> current_player;
        brd::board chess_board;
        game_status status;
        std::vector<std::string> moves_played;

        static int games_played;
        static int player_one_wins;
        static int player_two_wins;
        static int draws;

    public:
        // Constructors
        chess_game(plr::human_player player_one, std::shared_ptr<plr::player> player_two) :
            current_player{std::make_shared<plr::human_player>(player_one)}, 
            chess_board(), status{active} {
            
            players.push_back(current_player);
            players.push_back(std::move(player_two));
        }
        // Destructor
        ~chess_game(){}

        // Member functions
        std::shared_ptr<plr::player> get_current_player() { return current_player; }
        game_status get_game_status() { return status; }
        brd::board get_chess_board() { return chess_board; }
        bool has_ended() { return status == checkmate || status == draw; }
        void get_next_player_ready();
        bool current_player_make_a_move(bool in_check);
        void promote_pawn_if_possible();
        void update_game_status();
        void game_over();
        void save_game();
        void load_game();
        static void display_stats(std::string player_one_name, std::string player_two_name);
    };
}

#endif