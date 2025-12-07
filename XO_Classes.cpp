/**
 * @file XO_Classes.cpp
 * @brief Implementations of multiple Tic-Tac-Toe (X-O) variants and their UIs.
 *
 * This file contains the concrete board and UI implementations for:
 * - Infinity 3x3 X-O
 * - 4x4 sliding X-O
 * - Numerical X-O (1–9 forming 15)
 * - SUS game (forming the word "SUS")
 * - Inverse X-O (losing condition)
 * - Pyramid X-O
 * - Word-based X-O with dictionary
 * - Connect 4
 * - Memory X-O (hidden board)
 * - Diamond 7x7 Tic-Tac-Toe
 * - Obstacle Tic-Tac-Toe
 * - 5x5 X-O (score by number of 3-in-a-row lines)
 * - Ultimate Tic-Tac-Toe (9x9 meta-board)
 */

#include <iostream>
#include <iomanip>
#include <cctype> 
#include <queue>
#include <utility>
#include "XO_Classes.h"
#include <map>
#include <algorithm>
#include "dic.h"

using namespace std;

/**
 * @brief Default constructor for the classic 3x3 X-O UI.
 *
 * Initializes the base UI with a welcome message and 3 rounds.
 */
XO_UI::XO_UI() : UI<char>("Welcome to FCAI Infinty X-O Game", 3) {}

/**
 * @brief Default constructor for the 4x4 X-O UI.
 */
XO_UI4::XO_UI4() :UI<char>("Welcome to FCAI 4x4 X-O Game", 3) {}

/**
 * @brief Default constructor for the Numerical X-O UI.
 */
XO_Num_UI::XO_Num_UI() :UI<char>("Welcome to FCAI Numerical X-O Game", 3) {}

/**
 * @brief Default constructor for the Pyramid X-O UI.
 */
Pyramid_XO_UI::Pyramid_XO_UI() :UI<char>("Welcome to FCAI Pyramid X-O Game", 3) {}

/**
 * @brief Default constructor for the WORD X-O UI.
 */
XO_UI_WORD::XO_UI_WORD() : UI<char>("Welcome to FCAI WORD X-O Game", 3) {}

/**
 * @brief Default constructor for the 5x5 X-O UI.
 */
XO_UI_5::XO_UI_5() : UI<char>("Welcome to FCAI 5x5 X-O Game", 3) {}

/**
 * @brief Default constructor for the Memory X-O UI.
 */
Memo_XO_UI::Memo_XO_UI() : UI<char>("Welcome to FCAI X-O Game by Dr El-Ramly", 3) {}

/**
 * @brief Default constructor for Obstacle Tic Tac Toe UI.
 */
obstacles_XO_UI::obstacles_XO_UI() : UI<char>("Welcome to Obstacle Tic Tac Toe!", 3) {}

/**
 * @brief Default constructor for Ultimate Tic Tac Toe UI.
 */
Ultimate_X_O_UI::Ultimate_X_O_UI() : UI<char>("Welcome to Ultimate Tic Tac Toe!", 3) {}

//--------------------------------------- Infinty_X_O_Board Implementation

/**
 * @brief Constructs a 3x3 infinity X-O board and clears all cells.
 *
 * The board starts empty and uses @c blank_symbol for all positions.
 */
X_O_Board::X_O_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Updates the Infinity X-O board with a new move.
 *
 * For this variant, only the last 6 moves remain on the board. When the
 * number of moves exceeds 6, the oldest move is removed.
 *
 * @param move Pointer to the move containing coordinates and symbol.
 * @return @c true if the move is valid and applied, otherwise @c false.
 */
bool X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. Validate move logic
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move (optional, rarely used in this game type)
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            // 2. Apply the new move
            n_moves++;
            board[x][y] = toupper(mark);

            // 3. Add this VALID move to the internal queue
            movelist.push({ x, y });

            // 4. Infinity Logic: If more than 6 pieces (3 per player), remove the oldest
            if (n_moves > 6) {
                int a = movelist.front().first;
                int b = movelist.front().second;

                // Clear the oldest piece
                board[a][b] = blank_symbol;

                // Remove it from the queue history
                movelist.pop();
            }
        }
        return true;
    }
    return false;
}

/**
 * @brief Checks if the given player has a winning 3-in-a-row on the Infinity board.
 *
 * @param player Pointer to the player to check for a win.
 * @return @c true if the player has won, otherwise @c false.
 */
bool X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Checks if the Infinity X-O game is a draw.
 *
 * In this implementation, an Infinity game is never considered a draw.
 *
 * @param player Unused player pointer.
 * @return Always @c false.
 */
bool X_O_Board::is_draw(Player<char>* player) {
    return false;
}

/**
 * @brief Checks if the Infinity X-O game is over.
 *
 * The game ends when the given player wins. Draw is always @c false here.
 *
 * @param player Pointer to the player to check.
 * @return @c true if the game is over, otherwise @c false.
 */
bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Creates a player for the classic 3x3 Infinity X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol (e.g. 'X' or 'O').
 * @param type Player type (Human or Computer).
 * @return Pointer to the newly created player.
 */
Player<char>* XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the current player for the 3x3 Infinity X-O game.
 *
 * Human players enter coordinates manually, computer players choose random cells.
 *
 * @param player Pointer to the current player.
 * @return Pointer to the newly allocated move.
 */
Move<char>* XO_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move : ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }

    return new Move<char>(x, y, player->get_symbol());
}

//--------------------------------------- 4X4_X_O_Board Implementation

/**
 * @brief Constructs a 4x4 X-O board and initializes starting positions.
 *
 * The first and last rows are pre-filled with alternating 'X' and 'O' pieces.
 */
X_O_Board4::X_O_Board4() : Board(4, 4) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) {
            board[0][i] = 'O';
            board[3][i] = 'X';
        }
        else {
            board[0][i] = 'X';
            board[3][i] = 'O';
        }
    }
}

/**
 * @brief Updates the 4x4 sliding X-O board with a directional move.
 *
 * The move is expected to be a @c DirectedMove with a direction:
 * - 'L' (left), 'R' (right), 'U' (up), or 'D' (down).
 * The source cell must contain the player's mark, and the destination must be empty.
 *
 * @param move Pointer to a @c DirectedMove describing the move.
 * @return @c true if the move is valid and applied, otherwise @c false.
 */
bool X_O_Board4::update_board(Move<char>* move)
{
    DirectedMove* dm = static_cast<DirectedMove*>(move);
    if (!dm) return false;

    int x = dm->get_x();
    int y = dm->get_y();
    char d = dm->get_direction();
    char mark = dm->get_symbol();

    // Validate source
    if (x < 0 || x >= 4 || y < 0 || y >= 4)
        return false;

    if (board[x][y] != mark)
        return false;

    // Determine destination
    int nx = x;
    int ny = y;

    if (d == 'L') ny--;
    else if (d == 'R') ny++;
    else if (d == 'U') nx--;
    else if (d == 'D') nx++;
    else return false;

    // Bounds check
    if (nx < 0 || nx >= 4 || ny < 0 || ny >= 4)
        return false;

    // Destination must be empty
    if (board[nx][ny] != blank_symbol)
        return false;

    // Apply move
    board[nx][ny] = mark;
    board[x][y] = blank_symbol;

    return true;
}

/**
 * @brief Checks if the given player has a winning 3-in-a-row on the 4x4 board.
 *
 * All horizontal, vertical and diagonal triplets are considered.
 *
 * @param player Pointer to the player to check.
 * @return @c true if the player has a winning line, otherwise @c false.
 */
bool X_O_Board4::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym) ||
            (all_equal(board[i][1], board[i][2], board[i][3]) && board[i][1] == sym) ||
            (all_equal(board[1][i], board[2][i], board[3][i]) && board[1][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym) ||
        (all_equal(board[3][3], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[3][1], board[2][2], board[1][3]) && board[2][2] == sym) ||
        (all_equal(board[0][1], board[1][2], board[2][3]) && board[1][2] == sym) ||
        (all_equal(board[1][0], board[2][1], board[3][2]) && board[2][1] == sym) ||
        (all_equal(board[0][3], board[1][2], board[2][1]) && board[1][2] == sym) ||
        (all_equal(board[3][0], board[2][1], board[1][2]) && board[3][0] == sym)
        )

        return true;

    return false;
}

/**
 * @brief Creates a player for the 4x4 X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to the newly created player.
 */
Player<char>* XO_UI4::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating 4x4 X-O Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from a player in the 4x4 sliding X-O UI.
 *
 * For human players, the input format is:
 * @code
 * row col direction
 * @endcode
 * where direction is one of L, R, U, D.
 *
 * @param player Pointer to the current player.
 * @return Pointer to a newly allocated @c DirectedMove.
 */
Move<char>* XO_UI4::get_move(Player<char>* player) {
    int x, y;
    char direction;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "It's your turn " << player->get_name();
        cout << "\nEnter your cell for 4x4 and where to move (row col 0 to 3) , (L,R,U,D) : ";
        cin >> x >> y >> direction;

    }
    else {
        // computer random move
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        char dirs[4] = { 'L','R','U','D' };
        direction = dirs[rand() % 4];
    }

    return new DirectedMove(x, y, player->get_symbol(), direction);
}

/**
 * @brief Checks if the 4x4 sliding X-O game is a draw.
 *
 * This implementation always returns @c false.
 *
 * @param player Unused player pointer.
 * @return Always @c false.
 */
bool X_O_Board4::is_draw(Player<char>* player) {
    return false;
}

/**
 * @brief Checks whether the 4x4 X-O game is over.
 *
 * @param player Pointer to the player to check.
 * @return @c true if the player has won or the game is a draw, otherwise @c false.
 */
bool X_O_Board4::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- Num_X_O_Board Implementation

/**
 * @brief Constructs a 3x3 Numerical X-O board.
 *
 * Initializes all cells to blank and marks all digits 1–9 as unused.
 */
X_O_Num_Board::X_O_Num_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    for (char i = '1'; i <= '9'; i++) {
        used_numbers[i] = false;
    }
}

/**
 * @brief Updates the Numerical X-O board with a new move.
 *
 * - X player plays on even turns (n_moves % 2 == 0) and must place odd numbers.
 * - O player plays on odd turns and must place even numbers.
 * - Each number from '1' to '9' can be used at most once.
 *
 * @param move Pointer to the move including coordinates and digit.
 * @return @c true if the move is valid and applied, @c false otherwise.
 */
bool X_O_Num_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. Validate move logic
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move (optional, rarely used in this game type)
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            if (n_moves % 2 == 0) // X player
            {
                // Check if the number is odd
                if ((mark - '0') % 2 == 0 || used_numbers[mark - '0'])
                    return false; // Invalid move
            }
            else // O player
            {
                // Check if the number is even
                if ((mark - '0') % 2 != 0 || used_numbers[mark - '0'])
                    return false; // Invalid move
            }
            used_numbers[mark - '0'] = true;
            // 2. Apply the new move
            board[x][y] = (mark);
            n_moves++;
        }
        return true;
    }
    return false;
}

/**
 * @brief Checks if the Numerical X-O board has a winning line summing to 15.
 *
 * A win occurs if any row, column, or diagonal forms a sum of 15 using 3 digits.
 *
 * @param player Pointer to the player (unused for checking).
 * @return @c true if any line sums to 15, otherwise @c false.
 */
bool X_O_Num_Board::is_win(Player<char>* player) {
    auto notzero = [&](char a, char b, char c) {
        return a != blank_symbol && b != blank_symbol && c != blank_symbol;
        };
    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if (notzero(board[i][0], board[i][1], board[i][2]) &&
            (board[i][0] - '0' + board[i][1] - '0' + board[i][2] - '0') == 15)
            return true;
        if (notzero(board[0][i], board[1][i], board[2][i]) &&
            (board[0][i] - '0' + board[1][i] - '0' + board[2][i] - '0') == 15)
            return true;
    }
    // Check diagonals
    if (notzero(board[0][0], board[1][1], board[2][2]) &&
        (board[0][0] - '0' + board[1][1] - '0' + board[2][2] - '0') == 15)
        return true;
    if (notzero(board[0][2], board[1][1], board[2][0]) &&
        (board[0][2] - '0' + board[1][1] - '0' + board[2][0] - '0') == 15)
        return true;
    return false;
}

/**
 * @brief Gets a move for the Numerical X-O UI.
 *
 * Human players input (row, column, number). Computer players choose a random
 * cell and a random digit from '1' to '9'.
 *
 * @param player Pointer to the current player.
 * @return Pointer to the newly allocated move.
 */
Move<char>* XO_Num_UI::get_move(Player<char>* player) {
    int x, y;
    char mark;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move (row column number): ";
        cin >> x >> y >> mark;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        mark = (rand() % 9) + '1'; // Random number between '1' and '9'

    }
    return new Move<char>(x, y, mark);
}

/**
 * @brief Checks if the Numerical X-O game is a draw.
 *
 * A draw occurs when all 9 moves are played and there is no winning line
 * summing to 15.
 *
 * @param player Pointer to player for checking win.
 * @return @c true if draw, otherwise @c false.
 */
bool X_O_Num_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

/**
 * @brief Checks if the Numerical X-O game is over.
 *
 * @param player Pointer to the player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool X_O_Num_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Creates a player for Numerical X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to the created player.
 */
Player<char>* XO_Num_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Numerical XO Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;

    return new Player<char>(name, symbol, type);
}

//--------------------------------------- SUS_X_O_Board Implementation

/**
 * @brief Maps three board positions to an index representing a specific line.
 *
 * The function identifies which of the 8 possible 3-in-a-row lines
 * (3 rows, 3 columns, 2 diagonals) the triplet belongs to.
 *
 * @param a First cell coordinate.
 * @param b Second cell coordinate.
 * @param c Third cell coordinate.
 * @return An integer ID (1–8) corresponding to the line.
 */
int SUS_Board::check(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    if (a.first == 0 && a.second == 0 && b.first == 0 && b.second == 1 && c.first == 0 && c.second == 2) {
        return 1;
    }
    if (a.first == 1 && a.second == 0 && b.first == 1 && b.second == 1 && c.first == 1 && c.second == 2) {
        return 2;
    }
    if (a.first == 2 && a.second == 0 && b.first == 2 && b.second == 1 && c.first == 2 && c.second == 2) {
        return 3;
    }
    if (a.first == 0 && a.second == 0 && b.first == 1 && b.second == 0 && c.first == 2 && c.second == 0) {
        return 4;
    }
    if (a.first == 0 && a.second == 1 && b.first == 1 && b.second == 1 && c.first == 2 && c.second == 1) {
        return 5;
    }
    if (a.first == 0 && a.second == 2 && b.first == 1 && b.second == 2 && c.first == 2 && c.second == 2) {
        return 6;
    }
    if (a.first == 0 && a.second == 0 && b.first == 1 && b.second == 1 && c.first == 2 && c.second == 2) {
        return 7;
    }
    if (a.first == 0 && a.second == 2 && b.first == 1 && b.second == 1 && c.first == 2 && c.second == 0) {
        return 8;
    }
    return 0;
}

/**
 * @brief Global array marking whether a particular SUS line has been counted.
 *
 * Each index 1–8 corresponds to a specific row, column, or diagonal.
 */
bool taken[9]{};

/**
 * @brief Constructs a 3x3 SUS board initialized with blanks.
 */
SUS_Board::SUS_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Updates the SUS board with a new move.
 *
 * Standard bounds and occupancy checks are applied. The symbol is converted
 * to upper case when placed.
 *
 * @param move Pointer to the move.
 * @return @c true if the move is valid and applied, otherwise @c false.
 */
bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. Validate move logic
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move (optional, rarely used in this game type)
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            // 2. Apply the new move
            n_moves++;
            board[x][y] = toupper(mark);

        }

        return true;
    }
    return false;
}

/**
 * @brief Checks if player U (second player) wins the SUS game.
 *
 * The game forms the word "SUS" on rows, columns, or diagonals.
 * The @c sus_count array is used to record the number of SUS lines
 * for each player, but the final winner is only decided at 9 moves.
 *
 * @param player Pointer to the player (used indirectly via @c n_moves).
 * @return @c true if player U has more SUS lines at the end, otherwise @c false.
 */
bool SUS_Board::is_win(Player<char>* player) {
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == 'S' && board[1][i] == 'U' && board[2][i] == 'S' && !taken[check({ 0,i }, { 1,i }, { 2,i })]) {
            sus_count[n_moves % 2]++;
            taken[check({ 0,i }, { 1,i }, { 2,i })] = 1;
        }
        if (board[i][0] == 'S' && board[i][1] == 'U' && board[i][2] == 'S' && !taken[check({ i,0 }, { i,1 }, { i,2 })]) {
            sus_count[n_moves % 2]++;
            taken[check({ i,0 }, { i,1 }, { i,2 })] = 1;
        }
    }
    if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S' && !taken[check({ 0,0 }, { 1,1 }, { 2,2 })]) {
        sus_count[n_moves % 2]++;
        taken[check({ 0,0 }, { 1,1 }, { 2,2 })] = 1;
    }
    if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S' && !taken[check({ 0,2 }, { 1,1 }, { 2,0 })]) {
        sus_count[n_moves % 2]++;
        taken[check({ 0,2 }, { 1,1 }, { 2,0 })] = 1;
    }
    if (n_moves == 9 && sus_count[1] > sus_count[0]) {
        return true;
    }
    cout << sus_count[1] << "      " << sus_count[0] << '\n';

    return false;
}

/**
 * @brief Checks if the given player loses the SUS game.
 *
 * Loss occurs when the opponent has more SUS lines after 9 moves.
 *
 * @param player Pointer to the player (index used in @c sus_count).
 * @return @c true if the player loses, otherwise @c false.
 */
bool SUS_Board::is_lose(Player<char>* player) {
    if (n_moves == 9 && sus_count[1] < sus_count[0]) {
        return true;
    }
    return false;
}

/**
 * @brief Checks if the SUS game is a draw.
 *
 * Draw occurs when both players have the same number of SUS lines
 * after all 9 moves.
 *
 * @param player Pointer to player (unused for logic).
 * @return @c true if draw, otherwise @c false.
 */
bool SUS_Board::is_draw(Player<char>* player) {
    if (n_moves == 9 && sus_count[0] == sus_count[1]) {
        return true;
    }
    return false;
}

/**
 * @brief Checks if the SUS game is over.
 *
 * The game is over after the 9th move regardless of result.
 *
 * @param player Pointer to player.
 * @return Always @c true when @c n_moves == 9.
 */
bool SUS_Board::game_is_over(Player<char>* player) {
    return (n_moves == 9);
}

//--------------------------------------- SUS_UI Implementation

/**
 * @brief Default constructor for SUS game UI.
 */
SUS_UI::SUS_UI() : UI<char>("Welcome to our SUS Game", 3) {}

/**
 * @brief Creates a player for the SUS game UI.
 *
 * @param name Player name.
 * @param symbol Player symbol ('S' or 'U').
 * @param type Player type.
 * @return Pointer to new player.
 */
Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the current player in SUS UI.
 *
 * @param player Pointer to the player.
 * @return Pointer to the new move.
 */
Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

/**
 * @brief Sets up the two SUS players (S and U) via UI prompts.
 *
 * @return Dynamic array of two @c Player<char>* (Player S, Player U).
 */
Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameS = get_player_name("Player S");
    PlayerType typeS = get_player_type_choice("Player S", type_options);
    players[0] = create_player(nameS, static_cast<char>('S'), typeS);

    string nameU = get_player_name("Player U");
    PlayerType typeU = get_player_type_choice("Player U", type_options);
    players[1] = create_player(nameU, static_cast<char>('U'), typeU);

    return players;
}

//--------------------------------------Inverse_X_O_Board Implementation

/**
 * @brief Constructs an inverse 3x3 X-O board initialized to blanks.
 *
 * Inverse X-O is a variant where forming a 3-in-a-row causes a loss.
 */
X_O_Inverse_Board::X_O_Inverse_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Updates the inverse X-O board with a move.
 *
 * Standard bounds and occupancy checks are applied and symbols are uppercased.
 * A move with symbol 0 is treated as an undo.
 *
 * @param move Pointer to the move.
 * @return @c true if move is valid and applied, otherwise @c false.
 */
bool X_O_Inverse_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

/**
 * @brief Checks whether the given player has lost in inverse X-O.
 *
 * A loss occurs if the player forms a 3-in-a-row (rows, columns, diagonals).
 *
 * @param player Pointer to the player.
 * @return @c true if the player has lost, otherwise @c false.
 */
bool X_O_Inverse_Board::is_lose(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Checks if the inverse X-O game is a draw.
 *
 * Draw occurs when all 9 moves are played and the player has not lost.
 *
 * @param player Pointer to the player to check loss.
 * @return @c true if draw, otherwise @c false.
 */
bool X_O_Inverse_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_lose(player));
}

/**
 * @brief Checks whether the inverse X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if player has lost or the game is a draw, otherwise @c false.
 */
bool X_O_Inverse_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

/**
 * @brief Default constructor for inverse X-O UI.
 */
XO_Inverse_UI::XO_Inverse_UI() : UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}

/**
 * @brief Creates a player for inverse X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to the created player.
 */
Player<char>* XO_Inverse_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the current player in inverse X-O UI.
 *
 * @param player Pointer to the player.
 * @return Pointer to the new move.
 */
Move<char>* XO_Inverse_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

//--------------------------------------- Pyramid_X_O_Board Implementation

/**
 * @brief Constructs a Pyramid X-O board (3x5) and initializes the pyramid shape.
 *
 * Invalid cells are represented with '*' on the internal board.
 */
Pyramid_X_O_Board::Pyramid_X_O_Board() : Board(3, 5) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    board[0][0] = '*'; board[0][1] = '*'; board[0][3] = '*'; board[0][4] = '*';
    board[1][0] = '*'; board[1][4] = '*';
}

/**
 * @brief Updates the Pyramid X-O board with a move.
 *
 * Bounds and occupancy are checked; symbol is uppercased.
 *
 * @param move Pointer to the move.
 * @return @c true if the move is valid and applied, otherwise @c false.
 */
bool Pyramid_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0
            )) {
        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

/**
 * @brief Checks if the given player has a winning triple in Pyramid X-O.
 *
 * The win patterns follow lines inside the pyramid.
 *
 * @param player Pointer to the player.
 * @return @c true if player has a winning line, otherwise @c false.
 */
bool Pyramid_X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };
    // Check pyramid rows
    if (all_equal(board[0][2], board[1][1], board[2][0]) && board[2][0] == sym)
        return true;
    if (all_equal(board[1][1], board[1][2], board[1][3]) && board[1][3] == sym)
        return true;
    if (all_equal(board[1][3], board[0][2], board[2][4]) && board[1][3] == sym)
        return true;
    if (all_equal(board[0][2], board[1][2], board[2][2]) && board[2][2] == sym)
        return true;
    for (int i = 0; i < 3; ++i) {
        if ((all_equal(board[2][i], board[2][i + 1], board[2][i + 2]) && board[2][i] == sym))
            return true;
    }
    return false;
}

/**
 * @brief Checks if the given player loses in Pyramid X-O.
 *
 * This variant does not define a losing condition separate from not winning.
 *
 * @param player Pointer to player.
 * @return Always @c false.
 */
bool Pyramid_X_O_Board::is_lose(Player<char>* player) {
    return false;
}

/**
 * @brief Checks if the Pyramid X-O game is a draw.
 *
 * Draw occurs when 9 moves have been played with no winner.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool Pyramid_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

/**
 * @brief Checks if Pyramid X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool Pyramid_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Creates a player for Pyramid X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* Pyramid_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Pyramid X-O Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for Pyramid X-O.
 *
 * @param player Pointer to the player.
 * @return Pointer to the new move.
 */
Move<char>* Pyramid_XO_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter your move for Pyramid X-O (row col): ";
        cin >> x >> y;
    }
    else {
        // computer random move
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

//--------------------------------------- Word_X_O_Board Implementation

/**
 * @brief Constructs a 3x3 WORD X-O board with all cells blank.
 */
X_O_Board_WORD::X_O_Board_WORD() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Creates a player for WORD X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* XO_UI_WORD::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for WORD X-O.
 *
 * Human players input (row, column, letter), and computer players choose random
 * positions and random letters.
 *
 * @param player Pointer to the player.
 * @return Pointer to the new move.
 */
Move<char>* XO_UI_WORD::get_move(Player<char>* player) {
    int x, y;
    char mark;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move ( row column word ) : ";
        cin >> x >> y >> mark;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        mark = 'A' + rand() % 26;
    }

    return new Move<char>(x, y, mark);
}

/**
 * @brief Updates the WORD X-O board with a new symbol.
 *
 * @param move Pointer to the move with coordinates and character.
 * @return @c true if move is valid and applied, otherwise @c false.
 */
bool X_O_Board_WORD::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move (optional, rarely used in this game type)
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            // 2. Apply the new move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

/**
 * @brief Checks if any row, column, or diagonal forms a valid dictionary word.
 *
 * Both normal and reversed directions are checked against the dictionary
 * set @c s.
 *
 * @param player Pointer to player (unused in logic).
 * @return @c true if any 3-letter line forms a valid word, otherwise @c false.
 */
bool X_O_Board_WORD::is_win(Player<char>* player) {

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        string e;
        string t;
        e.push_back(board[i][0]);
        e.push_back(board[i][1]);
        e.push_back(board[i][2]);
        t.push_back(board[0][i]);
        t.push_back(board[1][i]);
        t.push_back(board[2][i]);
        if (s.find(e) != s.end() || s.find(t) != s.end()) {
            return true;
        }

        reverse(e.begin(), e.end());
        reverse(t.begin(), t.end());

        if (s.find(e) != s.end() || s.find(t) != s.end()) {
            return true;
        }
        e.clear();
        t.clear();
    }

    // Check diagonals
    string e;
    string t;
    e.push_back(board[0][0]);
    e.push_back(board[1][1]);
    e.push_back(board[2][2]);
    t.push_back(board[0][2]);
    t.push_back(board[1][1]);
    t.push_back(board[2][0]);
    if (s.find(e) != s.end() || s.find(t) != s.end()) {
        return true;
    }
    reverse(e.begin(), e.end());
    reverse(t.begin(), t.end());

    if (s.find(e) != s.end() || s.find(t) != s.end()) {
        return true;
    }
    e.clear();
    t.clear();

    return false;
}

/**
 * @brief Checks if the WORD X-O game is a draw.
 *
 * A draw occurs when 9 moves are played and there is no valid word.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool X_O_Board_WORD::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

/**
 * @brief Checks whether the WORD X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool X_O_Board_WORD::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


//--------------------------------------- Connect4 Implementation

/**
 * @brief Constructs a 6x7 Connect 4 board initialized to blanks.
 */
Connect4_Board::Connect4_Board() : Board(6, 7) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Updates the Connect 4 board with a new move.
 *
 * A piece must either be on the bottom row or sit on top of another piece.
 *
 * @param move Pointer to the move containing (row, column, symbol).
 * @return @c true if move is valid and applied, otherwise @c false.
 */
bool Connect4_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0) && (x == 5 || (x < 5 && board[x + 1][y] != blank_symbol))) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

/**
 * @brief Checks if the given player has 4 in a row in Connect 4.
 *
 * All horizontal, vertical, and diagonal lines of length 4 are checked.
 *
 * @param player Pointer to the player.
 * @return @c true if the player has a connect-4, otherwise @c false.
 */
bool Connect4_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c, char d) {
        return a == b && b == c && c == d && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            if (all_equal(board[0 + i][j], board[1 + i][j], board[2 + i][j], board[3 + i][j]) && board[i][j] == sym) {
                return true;//column
            }
        }
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            if (all_equal(board[i][0 + j], board[i][1 + j], board[i][2 + j], board[i][3 + j]) && board[i][j] == sym) {
                return true;//row
            }
        }
    }

    // Check diagonals
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (all_equal(board[i][j + 3], board[i + 1][j + 2], board[i + 2][j + 1], board[i + 3][j]) && board[i][j + 3] == sym) {
                return true;
            }
            if (all_equal(board[i][3 - j], board[i][4 - j], board[i][5 - j], board[i][6 - j]) && board[i][3 - j] == sym) {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Checks if the Connect 4 game is a draw.
 *
 * Draw occurs when the board is full (42 moves) with no winner.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool Connect4_Board::is_draw(Player<char>* player) {
    return (n_moves == 42 && !is_win(player));
}

/**
 * @brief Checks whether the Connect 4 game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool Connect4_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Default constructor for Connect 4 UI.
 */
Connect4_UI::Connect4_UI() : UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}

/**
 * @brief Creates a player for the Connect 4 UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* Connect4_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for Connect 4.
 *
 * @param player Pointer to the player.
 * @return Pointer to the new move.
 */
Move<char>* Connect4_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 5) and (0 to 6): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

//--------------------------------------- Memo_XO_Classes.cpp

/**
 * @brief Constructs a Memory X-O board and initializes the revealed matrix.
 *
 * The public board shows '#' for hidden cells, while @c revealed stores
 * the actual underlying symbols.
 */
Memo_X_O_Board::Memo_X_O_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    revealed = vector<vector<char>>(3, vector<char>(3, blank_symbol));
}

/**
 * @brief Updates the Memory X-O board with a new hidden move.
 *
 * The symbol is placed in @c revealed, whereas the public board
 * simply shows '#'.
 *
 * @param move Pointer to the move.
 * @return @c true if the move is valid and applied, otherwise @c false.
 */
bool Memo_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // bounds check
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // internal real board must be empty
    if (revealed[x][y] != blank_symbol)
        return false;

    // place REAL symbol
    revealed[x][y] = toupper(mark);

    // board stays hidden
    board[x][y] = '#';

    n_moves++;
    return true;
}

/**
 * @brief Checks whether the given player has a winning 3-in-a-row on the hidden board.
 *
 * The check is done on the @c revealed array, not the public board.
 *
 * @param player Pointer to the player.
 * @return @c true if player has a winning line, otherwise @c false.
 */
bool Memo_X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };
    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(revealed[i][0], revealed[i][1], revealed[i][2]) && revealed[i][0] == sym) ||
            (all_equal(revealed[0][i], revealed[1][i], revealed[2][i]) && revealed[0][i] == sym))
            return true;
    }
    // Check diagonals
    if ((all_equal(revealed[0][0], revealed[1][1], revealed[2][2]) && revealed[1][1] == sym) ||
        (all_equal(revealed[0][2], revealed[1][1], revealed[2][0]) && revealed[1][1] == sym))
        return true;
    return false;
}

/**
 * @brief Checks if the Memory X-O game is a draw.
 *
 * Draw occurs after 9 moves with no winner.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool Memo_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

/**
 * @brief Checks whether the Memory X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool Memo_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Creates a player for Memory X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* Memo_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for Memory X-O.
 *
 * @param player Pointer to player.
 * @return Pointer to new move.
 */
Move<char>* Memo_XO_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move (row column): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

//--------------------------------------- Diamond_X_O_Board Implementation

/**
 * @brief Constructs a 7x7 Diamond X-O board with a diamond-shaped valid region.
 *
 * Valid cells (forming a diamond) are initialized with '.', invalid ones with '$'.
 */
Diamond_X_O_Board::Diamond_X_O_Board() :Board<char>(7, 7) {
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            bool valid = (abs(i - 3) + abs(j - 3) <= 3);
            valid_cell[i][j] = valid;

            if (valid)
                board[i][j] = '.';
            else
                board[i][j] = '$';
        }
    }

    n_moves = 0;
}

/**
 * @brief Updates the Diamond X-O board with a new move.
 *
 * The move must target a valid diamond cell that is empty.
 *
 * @param move Pointer to the move.
 * @return @c true if move valid and applied, otherwise @c false.
 */
bool Diamond_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= 7 || y < 0 || y >= 7) return false;
    if (!valid_cell[x][y]) return false;
    if (board[x][y] != blank_symbol) return false;

    board[x][y] = toupper(mark);
    ++n_moves;
    return true;
}

/**
 * @brief Collects all lines of a given length formed by the player's symbol.
 *
 * The lines are checked in 4 directions: horizontal, vertical, and both diagonals.
 *
 * @param player Pointer to the player.
 * @param len Desired line length (e.g., 3 or 4).
 * @return Vector of lines, each line being a list of (row, col) points.
 */
std::vector<Diamond_X_O_Board::Line>
Diamond_X_O_Board::collect_lines(Player<char>* player, int len) const {

    std::vector<Line> lines;
    char sym = player->get_symbol();

    const int dirs[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (!valid_cell[i][j]) continue;

            for (int v = 0; v < 4; ++v) {
                int dx = dirs[v][0];
                int dy = dirs[v][1];

                Line line;
                bool ok = true;

                for (int k = 0; k < len; ++k) {
                    int nx = i + dx * k;
                    int ny = j + dy * k;


                    if (nx < 0 || nx >= 7 || ny < 0 || ny >= 7) {
                        ok = false; break;
                    }

                    if (!valid_cell[nx][ny]) {
                        ok = false; break;
                    }

                    if (board[nx][ny] != sym) {
                        ok = false; break;
                    }

                    line.emplace_back(nx, ny);
                }

                if (ok) {

                    std::sort(line.begin(), line.end());

                    lines.push_back(line);
                }
            }
        }
    }

    return lines;
}

/**
 * @brief Checks if the player has won in the Diamond X-O game.
 *
 * A win occurs if the player has at least one line of length 3 and
 * one line of length 4 that intersect at exactly one common cell
 * and are not collinear (different directions).
 *
 * @param player Pointer to the player.
 * @return @c true if the win condition is satisfied, otherwise @c false.
 */
bool Diamond_X_O_Board::is_win(Player<char>* player) {
    auto lines3 = collect_lines(player, 3);
    auto lines4 = collect_lines(player, 4);

    if (lines3.empty() || lines4.empty()) return false;

    auto get_dir = [](const Line& L) -> std::pair<int, int> {
        if (L.size() < 2) return { 0,0 };
        return { L[1].first - L[0].first, L[1].second - L[0].second };
        };

    for (const auto& L3 : lines3) {
        auto d3 = get_dir(L3);

        for (const auto& L4 : lines4) {
            auto d4 = get_dir(L4);


            if (d3 == d4) continue;


            if (d3.first == -d4.first && d3.second == -d4.second) continue;

            int inter = 0;
            for (auto& p3 : L3)
                for (auto& p4 : L4)
                    if (p3 == p4) ++inter;


            if (inter == 1)
                return true;
        }
    }

    return false;
}

/**
 * @brief Checks if the Diamond X-O game is a draw.
 *
 * Draw occurs when at least 25 moves have been played and no win.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool Diamond_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves >= 25 && !is_win(player));
}

/**
 * @brief Checks whether the Diamond X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool Diamond_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Default constructor for Diamond X-O UI.
 */
Diamond_X_O_UI::Diamond_X_O_UI() : UI<char>("Welcome to Diamond 7x7 Tic-Tac-Toe", 3) {}

/**
 * @brief Creates a player for the Diamond X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* Diamond_X_O_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Diamond Player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for Diamond X-O UI.
 *
 * @param player Pointer to player.
 * @return Pointer to new move.
 */
Move<char>* Diamond_X_O_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "Enter move (row col 0–6): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else {

        int rx, ry;
        Board<char>* b = player->get_board_ptr();
        do {
            rx = rand() % 7;
            ry = rand() % 7;
        } while (!((Diamond_X_O_Board*)b)->is_valid_cell(rx, ry) || b->get_cell(rx, ry) != '.');

        return new Move<char>(rx, ry, player->get_symbol());
    }
}

//--------------------------------------------------- Obstacle_X_O_Board Implementation

/**
 * @brief Creates a player for Obstacle Tic Tac Toe UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* obstacles_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Obstacle Player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from player for Obstacle Tic Tac Toe.
 *
 * @param player Pointer to player.
 * @return Pointer to new move.
 */


/**
 * @brief Constructs a 6x6 Obstacle X-O board with all cells blank.
 */
obstacles_X_O_Board::obstacles_X_O_Board() : Board(6, 6) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Updates the Obstacle X-O board with a new move.
 *
 * Every second move (even @c n_moves), two random empty cells are turned into
 * obstacles ('#'). Obstacles cannot be used again.
 *
 * @param move Pointer to the move.
 * @return @c true if update succeeded, otherwise @c false.
 */
bool obstacles_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {
        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
            if (n_moves % 2 == 0) {
                int obstacles_placed = 0;
                while (obstacles_placed < 2) {
                    int rx = rand() % rows;
                    int ry = rand() % columns;
                    if (board[rx][ry] == blank_symbol) {
                        board[rx][ry] = obstacle_symbol;
                        obstacles_placed++;
                    }
                }
            }
        }
    }
    return true;
}

/**
 * @brief Checks if the given player has a 4-in-a-row on Obstacle X-O board.
 *لا 
 * All horizontal, vertical and diagonal triples are checked, ignoring obstacles.
 *
 * @param player Pointer to player.
 * @return @c true if player has winning line, otherwise @c false.
 */
bool obstacles_X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    // 4 in a row helper
    auto all_equal4 = [&](char a, char b, char c, char d) {
        return a == sym && a == b && b == c && c == d
            && a != blank_symbol && a != obstacle_symbol;
        };

    // Check rows (horizontal)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns - 3; ++j) { 
            if (all_equal4(board[i][j], board[i][j + 1], board[i][j + 2], board[i][j + 3]))
                return true;
        }
    }

    // Check columns (vertical)
    for (int j = 0; j < columns; ++j) {
        for (int i = 0; i < rows - 3; ++i) {     
            if (all_equal4(board[i][j], board[i + 1][j], board[i + 2][j], board[i + 3][j]))
                return true;
        }
    }

    // Check main diagonals (\)
    for (int i = 0; i < rows - 3; ++i) {
        for (int j = 0; j < columns - 3; ++j) {
            if (all_equal4(board[i][j], board[i + 1][j + 1],
                board[i + 2][j + 2], board[i + 3][j + 3]))
                return true;
        }
    }

    for (int i = 0; i < rows - 3; ++i) {
        for (int j = 3; j < columns; ++j) { 
            if (all_equal4(board[i][j], board[i + 1][j - 1],
                board[i + 2][j - 2], board[i + 3][j - 3]))
                return true;
        }
    }

    return false;
}


/**
 * @brief Checks if Obstacle X-O is a draw.
 *
 * Draw occurs when all 36 cells are used with no winner.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool obstacles_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves + 18 == 36 && !is_win(player));
}

/**
 * @brief Checks whether the Obstacle X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool obstacles_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Move<char>* obstacles_XO_UI::get_move(Player<char>* player) {
    int x, y;
    Board<char>* b = player->get_board_ptr();

    if (player->get_type() == PlayerType::HUMAN) {

        while (true) {
            cout << "Enter move (row col 0 to 5): ";
            cin >> x >> y;

            // bounds
            if (x < 0 || x >= 6 || y < 0 || y >= 6) {
                cout << "Invalid position! Try again.\n";
                continue;
            }

            // obstacle
            if (b->get_cell(x, y) == '#') {
                cout << "That cell is an obstacle (#). Choose another.\n";
                continue;
            }

            // not empty
            if (b->get_cell(x, y) != '.') {
                cout << "Cell already used. Try another.\n";
                continue;
            }

            break;
        }
    }
    else {
        // computer random valid move
        do {
            x = rand() % 6;
            y = rand() % 6;
        } while (b->get_cell(x, y) != '.');
    }

    return new Move<char>(x, y, player->get_symbol());
}

// ------------------------------ 5x5 X-O board 

/**
 * @brief Creates a player for 5x5 X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* XO_UI_5::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for 5x5 X-O.
 *
 * @param player Pointer to player.
 * @return Pointer to new move.
 */
Move<char>* XO_UI_5::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move : ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }

    return new Move<char>(x, y, player->get_symbol());
}

/**
 * @brief Constructs a 5x5 X-O board with all cells blank.
 *
 * The game is scored by counting the number of 3-in-a-row lines for each player.
 */
X_O_Board_5::X_O_Board_5() : Board(5, 5) {
    for (int i = 0;i < rows;++i)
        for (int j = 0;j < columns;++j)
            board[i][j] = blank_symbol;
    n_moves = 0;
}

/**
 * @brief Updates the 5x5 X-O board with a new move.
 *
 * The board does not allow moves after 24 total moves.
 *
 * @param move Pointer to move.
 * @return @c true if move valid and applied, otherwise @c false.
 */
bool X_O_Board_5::update_board(Move<char>* move) {
    int x = move->get_x(), y = move->get_y();
    char mark = toupper(move->get_symbol());
    if (x < 0 || x >= rows || y < 0 || y >= columns || n_moves == 24) return false;
    if (board[x][y] != blank_symbol) return false;
    board[x][y] = mark;
    ++n_moves;
    return true;
}

/**
 * @brief Counts the number of 3-in-a-row lines for a given symbol on the 5x5 board.
 *
 * All horizontal, vertical and diagonal triplets are considered.
 *
 * @param sym The symbol to count lines for ('X' or 'O').
 * @return Number of 3-in-a-row occurrences.
 */
int X_O_Board_5::count_three_in_row(char sym) {
    int count = 0;
    int rows = 5, cols = 5;

    auto check_three = [&](char a, char b, char c) {
        return a == sym && b == sym && c == sym;
        };


    for (int r = 0; r < rows; r++) {
        for (int c = 0; c <= cols - 3; c++) {
            if (check_three(board[r][c], board[r][c + 1], board[r][c + 2])) {
                count++;
            }
        }
    }


    for (int c = 0; c < cols; c++) {
        for (int r = 0; r <= rows - 3; r++) {
            if (check_three(board[r][c], board[r + 1][c], board[r + 2][c])) {
                count++;
            }
        }
    }


    for (int r = 0; r <= rows - 3; r++) {
        for (int c = 0; c <= cols - 3; c++) {
            if (check_three(board[r][c], board[r + 1][c + 1], board[r + 2][c + 2])) {
                count++;
            }
        }
    }

    for (int r = 0; r <= rows - 3; r++) {
        for (int c = 2; c < cols; c++) {
            if (check_three(board[r][c], board[r + 1][c - 1], board[r + 2][c - 2])) {
                count++;
            }
        }
    }

    return count;
}

/**
 * @brief Checks if the given player wins in 5x5 X-O by having more lines.
 *
 * After at least 24 moves, the total 3-in-a-row counts for 'X' and 'O'
 * are compared. The player wins iff their symbol has more lines.
 *
 * @param player Pointer to player.
 * @return @c true if the player wins, otherwise @c false.
 */
bool X_O_Board_5::is_win(Player<char>* player) {
    if (n_moves < 24) return false;

    int xCount = count_three_in_row('X');
    int oCount = count_three_in_row('O');

    cout << xCount << " " << oCount << '\n';

    if (xCount > oCount)   return player->get_symbol() == 'X';
    if (oCount > xCount)   return player->get_symbol() == 'O';

    return false;
}

/**
 * @brief Checks if the given player loses in 5x5 X-O by having fewer lines.
 *
 * @param player Pointer to player.
 * @return @c true if the player loses, otherwise @c false.
 */
bool X_O_Board_5::is_lose(Player<char>* player) {
    if (n_moves < 24) return false;

    int xCount = count_three_in_row('X');
    int oCount = count_three_in_row('O');

    cout << xCount << " " << oCount << '\n';

    if (xCount > oCount)   return player->get_symbol() == 'O';
    if (oCount > xCount)   return player->get_symbol() == 'X';

    return false;
}

/**
 * @brief Checks if the 5x5 X-O game is a draw.
 *
 * Draw occurs after 24 moves when both players have the same line count.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool X_O_Board_5::is_draw(Player<char>* player) {
    if (n_moves < 24) return false;
    char me = toupper(player->get_symbol());
    char opp = (me == 'X') ? 'O' : 'X';
    cout << count_three_in_row(opp) << " " << count_three_in_row(me) << endl;
    return count_three_in_row(me) == count_three_in_row(opp);

}

/**
 * @brief Checks whether the 5x5 X-O game is over.
 *
 * Game is considered over after 24 moves, regardless of result.
 *
 * @param player Pointer to player.
 * @return @c true if game reached 24 moves, otherwise @c false.
 */
bool X_O_Board_5::game_is_over(Player<char>* player) {
    return n_moves >= 24;
}

//--------------------------------------------------- Ultimate_X_O_Board Implementation

/**
 * @brief Constructs a 9x9 Ultimate X-O board with all cells blank.
 *
 * The board is composed of 9 sub-boards (3x3 each) forming a meta-game.
 */
Ultimate_X_O_Board::Ultimate_X_O_Board() : Board(9, 9) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Prints the 3x3 meta-board of Ultimate X-O.
 *
 * Each meta cell represents the winner of a 3x3 sub-board:
 * - 'X' if X won,
 * - 'O' if O won,
 * - ' ' (space) otherwise.
 */
void Ultimate_X_O_Board::print_meta_board() {
    cout << "\nMeta Board (3x3):\n";
    cout << "+---+---+---+\n";
    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) {
            char cell = main_board[i][j];
            if (cell == '.') cell = ' ';
            cout << cell << " | ";
        }
        cout << "\n+---+---+---+\n";
    }
}

/**
 * @brief Updates the @c main_board meta-board based on sub-board winners.
 *
 * For each 3x3 block in the 9x9 board, a winner is determined and stored
 * in the corresponding meta cell.
 */
void Ultimate_X_O_Board::update_meta_board() {

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            main_board[i][j] = '.';

    auto check_winner = [&](int bi, int bj) -> char {
        int r = bi * 3;
        int c = bj * 3;

        for (char sym : {'X', 'O'}) {
            auto all_equal = [&](char a, char b, char c) {
                return a == b && b == c && a == sym;
                };

            // Rows
            for (int i = 0; i < 3; i++)
                if (all_equal(board[r + i][c], board[r + i][c + 1], board[r + i][c + 2]))
                    return sym;

            // Columns
            for (int j = 0; j < 3; j++)
                if (all_equal(board[r][c + j], board[r + 1][c + j], board[r + 2][c + j]))
                    return sym;

            // Diagonals
            if (all_equal(board[r][c], board[r + 1][c + 1], board[r + 2][c + 2]))
                return sym;

            if (all_equal(board[r][c + 2], board[r + 1][c + 1], board[r + 2][c]))
                return sym;
        }

        return '.';
        };

    for (int bi = 0; bi < 3; bi++)
        for (int bj = 0; bj < 3; bj++)
            main_board[bi][bj] = check_winner(bi, bj);
}

/**
 * @brief Updates the 9x9 Ultimate X-O board with a move.
 *
 * After updating a cell, the meta board is recomputed and printed.
 *
 * @param move Pointer to move.
 * @return @c true if move is valid and applied, otherwise @c false.
 */
bool Ultimate_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {
        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        update_meta_board();
        print_meta_board();
        return true;
    }
    return false;
}

/**
 * @brief Checks if the given player wins in the Ultimate X-O meta-game.
 *
 * The method:
 * 1. Determines which sub-boards the player has won and marks them on
 *    @c main_board.
 * 2. Checks if there is a 3-in-a-row on the meta-board for that player.
 *
 * @param player Pointer to player.
 * @return @c true if the player wins the meta-game, otherwise @c false.
 */
bool Ultimate_X_O_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // Lambda for checking three equal non-blank cells
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a == sym;
        };

    // Reset main_board (3x3 meta-board)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            main_board[i][j] = '.'; // or blank_symbol

    // ---- PHASE 1: Determine winners of all 3x3 sub-boards ----
    for (int bi = 0; bi < 3; bi++) {
        for (int bj = 0; bj < 3; bj++) {

            int r = bi * 3;
            int c = bj * 3;

            bool won = false;

            // Check rows
            for (int i = 0; i < 3; i++)
                if (all_equal(board[r + i][c], board[r + i][c + 1], board[r + i][c + 2]))
                    won = true;

            // Check columns
            for (int j = 0; j < 3; j++)
                if (all_equal(board[r][c + j], board[r + 1][c + j], board[r + 2][c + j]))
                    won = true;

            // Check diagonals
            if (all_equal(board[r][c], board[r + 1][c + 1], board[r + 2][c + 2]))
                won = true;
            if (all_equal(board[r][c + 2], board[r + 1][c + 1], board[r + 2][c]))
                won = true;

            if (won) {
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        if (board[r + i][c + j] == blank_symbol) {
                            board[r + i][c + j] = '#';
                            ++y;
                        }
                main_board[bi][bj] = sym;
            }
        }
    }

    // ---- PHASE 2: Check win in the meta-board (3x3) ----

    // Rows
    for (int i = 0; i < 3; i++)
        if (all_equal(main_board[i][0], main_board[i][1], main_board[i][2])) {
            update_meta_board();
            print_meta_board();
            return true;
        }

    // Columns
    for (int j = 0; j < 3; j++)
        if (all_equal(main_board[0][j], main_board[1][j], main_board[2][j])) {
            update_meta_board();
            print_meta_board();
            return true;
        }

    // Diagonals
    if (all_equal(main_board[0][0], main_board[1][1], main_board[2][2])) {
        update_meta_board();
        print_meta_board();
        return true;
    }

    if (all_equal(main_board[0][2], main_board[1][1], main_board[2][0])) {
        update_meta_board();
        print_meta_board();
        return true;
    }

    return false;
}

/**
 * @brief Checks if the Ultimate X-O game is a draw.
 *
 * The game is a draw when all 81 cells (including converted '#' cells) are
 * effectively used without a winner.
 *
 * @param player Pointer to player.
 * @return @c true if draw, otherwise @c false.
 */
bool Ultimate_X_O_Board::is_draw(Player<char>* player) {
    return ((n_moves + y) == 81 && !is_win(player));
}

/**
 * @brief Checks whether the Ultimate X-O game is over.
 *
 * @param player Pointer to player.
 * @return @c true if win or draw, otherwise @c false.
 */
bool Ultimate_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Creates a player for the Ultimate X-O UI.
 *
 * @param name Player name.
 * @param symbol Player symbol.
 * @param type Player type.
 * @return Pointer to created player.
 */
Player<char>* Ultimate_X_O_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Ultimate X-O Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a move from the player for Ultimate X-O.
 *
 * @param player Pointer to player.
 * @return Pointer to new move.
 */
Move<char>* Ultimate_X_O_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter your move for Ultimate X-O (row col): ";
        cin >> x >> y;
    }
    else {
        // computer random move
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
