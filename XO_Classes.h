#include "BoardGame_Classes.h"
#include <queue> // Include queue here
#include <utility> // For pair
#include <map>
#include <unordered_set>
#ifndef XO_CLASSES_H
#define XO_CLASSES_H
extern std::unordered_set<std::string> s;

using namespace std;

class X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
    // Move the queue inside the class
    queue<pair<int, int>> movelist;

public:
    X_O_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

class XO_UI : public UI<char> {
public:
    XO_UI();
    ~XO_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};


class X_O_Board4 : public Board<char> {
private:
    char blank_symbol = '.';

public:
    X_O_Board4();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool game_is_over(Player<char>* player);
};

class XO_UI4 : public UI<char> {
public:
    XO_UI4();
    ~XO_UI4() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

class X_O_Num_Board : public Board<char> {
private:
    char blank_symbol = '0';
    map<int, bool> used_numbers;
	

public:
    X_O_Num_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

class XO_Num_UI : public UI<char> {
    public:
    XO_Num_UI();
    ~XO_Num_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int sus_count[2] = { 0,0 };// count SUS made by each player to detect winner
public:
    SUS_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    int check(pair<int, int>, pair<int, int>, pair<int, int>);
};

class SUS_UI : public UI<char> {
public:
    SUS_UI();
    ~SUS_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Player<char>** setup_players()override;
    virtual Move<char>* get_move(Player<char>* player);
};
/**
 * @class X_O_Board
 * @brief Represents the Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Tic-Tac-Toe (X-O) game, including
 * move updates, win/draw detection, and display functions.
 *
 * @see Board
 */
class X_O_Inverse_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    /**
     * @brief Default constructor that initializes a 3x3 X-O board.
     */
    X_O_Inverse_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has won the game.
     * @param player Pointer to the player being checked.
     * @return true if the player has a winning line, false otherwise.
     */
    bool is_win(Player<char>* player) { return false; };

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in X-O logic).
     */
    bool is_lose(Player<char>*);

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all cells are filled and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);
};


/**
 * @class XO_UI
 * @brief User Interface class for the X-O (Tic-Tac-Toe) game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O–specific functionality for player setup and move input.
 *
 * @see UI
 */
class XO_Inverse_UI : public UI<char> {
public:
    /**
     * @brief Constructs an XO_UI object.
     *
     * Initializes the base `UI<char>` class with the welcome message "FCAI X-O".
     */
    XO_Inverse_UI();

    /**
     * @brief Destructor for XO_UI.
     */
    ~XO_Inverse_UI() {};

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol ('X' or 'O') assigned to the player.
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    virtual Move<char>* get_move(Player<char>* player);
};

class Pyramid_X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
public:
    Pyramid_X_O_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};
class Pyramid_XO_UI : public UI<char> {
    public:
    Pyramid_XO_UI();
    ~Pyramid_XO_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

class XO_UI_WORD : public UI<char> {
public:
    XO_UI_WORD();
    ~XO_UI_WORD() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};


class X_O_Board_WORD : public Board<char> {
private:
    char blank_symbol = '.';

public:
    X_O_Board_WORD();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool game_is_over(Player<char>* player);
};

class Connect4_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:

    Connect4_Board();


    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};


class Connect4_UI : public UI<char> {
public:
    Connect4_UI();
    ~Connect4_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};


#endif // XO_CLASSES_H

