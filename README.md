# Cpp_Object-Oriented_Chess_Game
 Command line chess game created utilizing object-oriented programming (OOP) in C++.
 
 Key OOP and C++ features:
  - Smart pointers, to implement the Resource Acquisition Is Initialization (RAII) idiom and use safer dynamic memory allocation.
  - Polymorphism, using an abstract 'chess_piece' base class with a virtual 'get_valid_moves' method, overridden in inherited class (e.g. rook, pawn, etc) to provide a single interface to objects of different types, when using a 'chess_board' with a vector of 'chess_piece' base class pointers.
  - Others including the use of exception handling, lambda functions and algorithms (to implement erase-remove idiom when removing 'valid' chess piece moves that would leave the player in check/checkmate.

 Key functionality:
  - Get valid moves for a selected piece, including special moves such as en passant, castling and pawn promotion.
  - Checking whether a player is in check or checkmate using a function to look at opposition player's possible next moves
  - Simple chess engine which randomly chooses a moe for single player mode
  - Load in chess games using Portable Game Notation (PGN) .pgn files
  - Save games as .pgn files
  - Keep track of multiple games played, using static 'game' class variables, to display number of games played and number won by each player upon exiting the game.
