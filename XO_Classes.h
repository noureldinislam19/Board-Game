#include "BoardGame_Classes.h"
#include <queue>      // For std::queue
#include <utility>    // For std::pair
#include <map>
#include <unordered_set>

#ifndef XO_CLASSES_H
#define XO_CLASSES_H

/**
 * @file XO_Classes.h
 * @brief Declarations for all FCAI Tic-Tac-Toe (X-O) game variants.
 *
 * This header collects the board and UI classes used by the different
 * Tic-Tac-Toe games (classic, numerical, SUS, diamond, obstacles, 5x5, etc.).
 * Each concrete game typically consists of:
 *  - a Board<char>-derived class that stores the state and implements the rules
 *  - a UI<char>-derived class that interacts with the player(s).
 */

extern std::unordered_set<std::string> s;

using namespace std;

/**
 * @class X_O_Board
 * @brief Infinity 3x3 Tic-Tac-Toe board.
 *
 * Implements the "infinity" X-O variant where only the last 6 moves
 * remain on the board. Older moves are automatically removed after the
 * 7th move is played.
 */
class X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
    /// Queue used to remember the order of moves so old ones can be removed.
    queue<pair<int, int>> movelist;

public:
    /// Constructs an empty 3x3 infinity board.
    X_O_Board();

    /**
     * @brief Updates the board with the given move.
     * @param move Pointer to the move to play (or undo if symbol == 0).
     * @return true if the move was valid and applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks whether the given player currently has 3 in a row.
     * @param player Player to check.
     * @return true if player has a winning line, false otherwise.
     */
    bool is_win(Player<char>* player);

    /// Infinity mode does not use "lose" condition explicitly.
    bool is_lose(Player<char>*) { return false; };

    /**
     * @brief Checks for a draw state (not used in this infinity variant).
     * @param player Player to test from their perspective.
     * @return Always false for this variant.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Checks if the game is over (win or draw).
     * @param player Player to test from their perspective.
     * @return true if game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);
};

/**
 * @class XO_UI
 * @brief UI class for the Infinity X-O game.
 *
 * Responsible for creating players and reading their moves for the
 * infinity 3x3 Tic-Tac-Toe variant.
 */
class XO_UI : public UI<char> {
public:
    /// Constructs the UI with an appropriate welcome message.
    XO_UI();
    ~XO_UI() {};

    /**
     * @brief Creates a player object.
     * @param name Player name.
     * @param symbol 'X' or 'O'.
     * @param type Player type (human or computer).
     * @return Newly allocated Player object.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Reads the next move from the given player.
     * @param player Player whose move is requested.
     * @return Newly allocated Move object.
     */
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class DirectedMove
 * @brief Specialized move that also stores a direction.
 *
 * Used by the 4x4 sliding X-O variant where pieces move one step in
 * a given direction (L, R, U, D) instead of being placed freely.
 */
class DirectedMove : public Move<char>
{
    /// Direction of movement: 'L', 'R', 'U', or 'D'.
    char direction;

public:
    /**
     * @brief Constructs a directed move.
     * @param x Row index.
     * @param y Column index.
     * @param symbol Player symbol to move.
     * @param dir Direction to move in ('L','R','U','D').
     */
    DirectedMove(int x, int y, char symbol, char dir)
        : Move<char>(x, y, symbol), direction(dir) {}

    /**
     * @brief Gets the direction of this move.
     * @return Stored direction character.
     */
    char get_direction() const {
        return direction;
    }
};

/**
 * @class X_O_Board4
 * @brief 4x4 sliding Tic-Tac-Toe board.
 *
 * Pieces start pre-placed on the first and last rows and players move
 * their symbols one step at a time trying to make 3-in-a-row.
 */
class X_O_Board4 : public Board<char> {
private:
    char blank_symbol = '.';

public:
    /// Initializes the 4x4 board with the starting configuration.
    X_O_Board4();

    /**
     * @brief Applies a directed move on the board.
     * @param move Pointer to a DirectedMove object.
     * @return true if the move was valid and applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has a winning line.
     */
    bool is_win(Player<char>* player);

    /// Draw is not used explicitly in this variant.
    bool is_draw(Player<char>* player);

    bool is_lose(Player<char>*) { return false; };

    /// Returns true if game is over (win or draw).
    bool game_is_over(Player<char>* player);
};

/**
 * @class XO_UI4
 * @brief UI for the 4x4 sliding Tic-Tac-Toe game.
 *
 * Reads a cell position and a direction from the player and converts
 * them into a DirectedMove.
 */
class XO_UI4 : public UI<char> {
public:
    XO_UI4();
    ~XO_UI4() {};

    /// Creates a player object for the 4x4 game.
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /// Gets a DirectedMove (row, column, direction) from the player.
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class X_O_Num_Board
 * @brief Numerical Tic-Tac-Toe board (magic-sum variant).
 *
 * Players place digits 1–9; one player uses odd numbers, the other
 * uses even numbers. A player "wins" if any row/column/diagonal
 * sums to 15 (magic square rule).
 */
class X_O_Num_Board : public Board<char> {
private:
    char blank_symbol = '0';
    /// Tracks which numbers 1–9 have been used already.
    map<int, bool> used_numbers;

public:
    /// Constructs an empty 3x3 numerical board and clears usage map.
    X_O_Num_Board();

    /**
     * @brief Places a digit on the board (respecting parity rules).
     * @param move Move containing (row, column, digit).
     * @return true if the move was valid and applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the board contains a line summing to 15.
     * @param player Player to test (symbol itself is not used).
     * @return true if any row/column/diagonal sums to 15.
     */
    bool is_win(Player<char>* player);

    bool is_lose(Player<char>*) { return false; };

    /// Returns true if the board is full and no magic-sum line exists.
    bool is_draw(Player<char>* player);

    /// Checks whether the game is over (win or draw).
    bool game_is_over(Player<char>* player);
};

/**
 * @class XO_Num_UI
 * @brief UI for the Numerical Tic-Tac-Toe game.
 *
 * Gets row, column and digit from the user and builds the Move object.
 */
class XO_Num_UI : public UI<char> {
public:
    XO_Num_UI();
    ~XO_Num_UI() {};

    /// Creates a player for the numerical variant.
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /// Reads (row, column, digit) from the player.
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class SUS_Board
 * @brief Board for the SUS word game.
 *
 * Players place letters 'S' and 'U' trying to create the word "SUS"
 * in multiple lines; the winner is the player with more SUS triples.
 */
class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    /// Count SUS made by each player to detect the winner.
    int sus_count[2] = { 0,0 };

public:
    SUS_Board();

    /// Places letter on board and updates move count.
    bool update_board(Move<char>* move);

    /**
     * @brief Counts new SUS lines and determines the winner.
     * @param player Player to test from their perspective.
     * @return true if this player has strictly more SUS triples.
     */
    bool is_win(Player<char>* player);

    /// Returns true if the other player has more SUS triples.
    bool is_lose(Player<char>*);

    /// Returns true if both players have equal SUS count at game end.
    bool is_draw(Player<char>* player);

    /// Game ends when all cells are filled.
    bool game_is_over(Player<char>* player);

    /**
     * @brief Encodes a triple of positions into an index [1..8].
     * @param a First cell (row, col).
     * @param b Second cell (row, col).
     * @param c Third cell (row, col).
     * @return Index representing which line this triple belongs to.
     */
    int check(pair<int, int>, pair<int, int>, pair<int, int>);
};

/**
 * @class SUS_UI
 * @brief UI for the SUS word game.
 *
 * Handles player creation (S vs U) and reading moves.
 */
class SUS_UI : public UI<char> {
public:
    SUS_UI();
    ~SUS_UI() {};

    /// Creates an 'S' or 'U' player.
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /// Sets up both players with proper symbols.
    Player<char>** setup_players() override;

    /// Reads a move (row, col) from the player.
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class X_O_Inverse_Board
 * @brief Represents the Tic-Tac-Toe game board (misère variant).
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Tic-Tac-Toe (X-O) game where
 * creating a 3-in-a-row is considered losing.
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
     * @return true if the player has a 3-in-a-row (losing condition).
     */
    bool is_lose(Player<char>*);

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all cells are filled and no player has lost, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over (lose or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);
};


/**
 * @class XO_Inverse_UI
 * @brief User Interface class for the Misère Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O–specific functionality for player setup and move input.
 *
 * @see UI
 */
class XO_Inverse_UI : public UI<char> {
public:
    /**
     * @brief Constructs an XO_Inverse_UI object.
     *
     * Initializes the base `UI<char>` class with a welcome message.
     */
    XO_Inverse_UI();

    /**
     * @brief Destructor for XO_Inverse_UI.
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

/**
 * @class Pyramid_X_O_Board
 * @brief Tic-Tac-Toe board with a pyramid-shaped set of valid cells.
 *
 * Only specific positions are playable; winning patterns follow the
 * pyramid geometry rather than a simple rectangle.
 */
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

/**
 * @class Pyramid_XO_UI
 * @brief UI for the Pyramid Tic-Tac-Toe game.
 */
class Pyramid_XO_UI : public UI<char> {
public:
    Pyramid_XO_UI();
    ~Pyramid_XO_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class XO_UI_WORD
 * @brief UI for the Word Tic-Tac-Toe game.
 *
 * Reads a letter from the user in addition to the cell coordinates.
 */
class XO_UI_WORD : public UI<char> {
public:
    XO_UI_WORD();
    ~XO_UI_WORD() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};


/**
 * @class X_O_Board_WORD
 * @brief Word Tic-Tac-Toe board.
 *
 * Each line on the board represents a 3-letter word. A player wins
 * if any row/column/diagonal forms a valid word from the dictionary.
 */
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

/**
 * @class Connect4_Board
 * @brief Standard 6x7 Connect Four board.
 *
 * Simulates gravity: discs must be placed on top of existing ones and
 * a player wins by connecting four of their symbols.
 */
class Connect4_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    /// Constructs an empty 6x7 Connect Four board.
    Connect4_Board();

    /// Places a disc if the move respects gravity and bounds.
    bool update_board(Move<char>* move);

    /// Checks if the given player has 4 connected symbols.
    bool is_win(Player<char>* player);

    bool is_lose(Player<char>*) { return false; };

    /// Checks for draw (board full with no winner).
    bool is_draw(Player<char>* player);

    /// Returns true if game ended (win or draw).
    bool game_is_over(Player<char>* player);
};


/**
 * @class Connect4_UI
 * @brief UI for the Connect Four game.
 */
class Connect4_UI : public UI<char> {
public:
    Connect4_UI();
    ~Connect4_UI() {};

    /// Creates a player for the Connect Four game.
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /// Reads a move (row, column) from the player.
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class Memo_X_O_Board
 * @brief Memory Tic-Tac-Toe board.
 *
 * Stores the real symbols in a hidden grid while showing only masked
 * cells ('#') to the players; win detection uses the hidden board.
 */
class Memo_X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
    /// Hidden "real" board storing actual symbols.
    vector<vector<char>> revealed;

public:
    Memo_X_O_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

/**
 * @class Memo_XO_UI
 * @brief UI for the Memory Tic-Tac-Toe game.
 */
class Memo_XO_UI : public UI<char> {
public:
    Memo_XO_UI();
    ~Memo_XO_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class Diamond_X_O_Board
 * @brief 7x7 diamond-shaped Tic-Tac-Toe board.
 *
 * Only cells inside a Manhattan-distance-3 diamond are valid. A player
 * wins by creating one 3-cell line and one 4-cell line that intersect
 * in exactly one cell.
 */
class Diamond_X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
public:
    /// Type alias for a line of cells.
    typedef std::vector<std::pair<int, int>> Line;

    Diamond_X_O_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }

    /// Marks which cells belong to the diamond.
    bool valid_cell[7][7];

    /**
     * @brief Checks whether a coordinate is a valid diamond cell.
     */
    bool is_valid_cell(int x, int y) const {
        return x >= 0 && x < 7 && y >= 0 && y < 7 && valid_cell[x][y];
    }

    /**
     * @brief Collects all lines of a given length made by the player.
     * @param player Player whose symbol is used.
     * @param len Desired line length (3 or 4).
     * @return Vector of lines, each line is a vector of coordinates.
     */
    std::vector<Line> collect_lines(Player<char>* player, int len) const;
};

/**
 * @class Diamond_X_O_UI
 * @brief UI for the Diamond 7x7 Tic-Tac-Toe game.
 */
class Diamond_X_O_UI : public UI<char> {
public:
    Diamond_X_O_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};


/**
 * @class obstacles_X_O_Board
 * @brief 6x6 Tic-Tac-Toe board with random obstacles.
 *
 * After every second move, two random cells turn into permanent
 * obstacles ('#'), shrinking the available space on the board.
 */
class obstacles_X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
    char obstacle_symbol = '#';
public:
    obstacles_X_O_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

/**
 * @class obstacles_XO_UI
 * @brief UI for the Obstacles Tic-Tac-Toe game.
 */
class obstacles_XO_UI : public UI<char> {
public:
    obstacles_XO_UI();
    ~obstacles_XO_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class X_O_Board_5
 * @brief 5x5 Tic-Tac-Toe board scored by number of 3-in-a-rows.
 *
 * After the board is almost full, each player's score is the number
 * of distinct 3-cell lines they have; the higher score wins.
 */
class X_O_Board_5 : public Board<char> {
public:
    X_O_Board_5();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool is_lose(Player<char>* player);
    bool game_is_over(Player<char>* player);
private:
    /**
     * @brief Counts all 3-in-a-row lines for a given symbol.
     * @param sym 'X' or 'O'.
     * @return Number of distinct 3-cell lines.
     */
    int count_three_in_row(char sym);

    char blank_symbol = '.';
};


/**
 * @class XO_UI_5
 * @brief UI for the 5x5 scoring Tic-Tac-Toe game.
 */
class XO_UI_5 : public UI<char> {
public:
    XO_UI_5();
    ~XO_UI_5() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class Ultimate_X_O_Board
 * @brief 9x9 Ultimate Tic-Tac-Toe board.
 *
 * The board is divided into 3x3 sub-boards; each sub-board behaves
 * like a mini Tic-Tac-Toe whose winner fills a cell in the meta-board.
 */
class Ultimate_X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';
    /// Meta-board of 3x3 representing winners of sub-boards.
    char main_board[3][3];
    /// Internal helper state used for sub-board targeting.
    int y = 0;
public:
    Ultimate_X_O_Board();

    /// Prints the current meta-board (3x3 winners).
    void print_meta_board();

    /// Updates the meta-board according to won sub-boards.
    void update_meta_board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_lose(Player<char>*) override { return false; }
};

/**
 * @class Ultimate_X_O_UI
 * @brief UI for the Ultimate Tic-Tac-Toe game.
 */
class Ultimate_X_O_UI : public UI<char> {
public:
    Ultimate_X_O_UI();

    /// Creates a player for the Ultimate Tic-Tac-Toe game.
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /// Reads a move from the player (board + cell).
    virtual Move<char>* get_move(Player<char>* player) override;

    /// Displays the 3x3 meta-board.
    void display_main_board(char main_board[3][3]);
};

#endif // XO_CLASSES_H

