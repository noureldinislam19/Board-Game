#include <iostream>
#include <iomanip>
#include <cctype> 
#include <queue>
#include <utility>
#include "XO_Classes.h"
#include <map>

using namespace std;

X_O_Board::X_O_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

//--------------------------------------- Infinty_X_O_Board Implementation
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

bool X_O_Board::is_draw(Player<char>* player) {
    return false;
}

bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

XO_UI::XO_UI() : UI<char>("Welcome to FCAI Infinty X-O Game", 3) {}

XO_UI4::XO_UI4() :UI<char>("Welcome to FCAI 4x4 X-O Game", 3){}

XO_Num_UI::XO_Num_UI() :UI<char>("Welcome to FCAI Numerical X-O Game", 3) {}

Player<char>* XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

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

//--------------------------------------- Numerical_X_O_Board Implementation
X_O_Board4::X_O_Board4() : Board(4, 4) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool X_O_Board4::update_board(Move<char>* move) {
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
Player<char>* XO_UI4::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating 4x4 X-O Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_UI4::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter your move for 4x4 (row col 0–3): ";
        cin >> x >> y;
    }
    else {
        // computer random move
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }

    return new Move<char>(x, y, player->get_symbol());
}

bool X_O_Board4::is_draw(Player<char>* player) {
    return (n_moves == 16 && !is_win(player));
}

bool X_O_Board4::game_is_over(Player<char>* player) {
	return is_win(player) || is_draw(player);
}

//--------------------------------------- Num_X_O_Board Implementation

X_O_Num_Board::X_O_Num_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}
bool X_O_Num_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    used_numbers [1, false] ; used_numbers [2, false]; used_numbers [3, false];
    used_numbers [4, false];  used_numbers [5, false]; used_numbers [6, false];
    used_numbers [7, false];  used_numbers [8, false]; used_numbers [9, false];

    // 1. Validate move logic
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move (optional, rarely used in this game type)
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            if(n_moves % 2 == 0) // X player
            {
                // Check if the number is odd
                if((mark - '0') % 2 == 0)
                    return false; // Invalid move
            }
            else // O player
            {
                // Check if the number is even
                if((mark - '0') % 2 != 0)
                    return false; // Invalid move
			}
            used_numbers[mark] = true;
			// 2. Apply the new move
			board[x][y] = (mark);
            n_moves++;
        }
        return true;
    }
    return false;
}

bool X_O_Num_Board::is_win(Player<char>* player) {
    auto all_equal = [&](int a, int b, int c) {
        return a == b && b == c && a != blank_symbol;
        };
    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if (int(board[i][0] + board[i][1] + board[i][2]) == 15 ||
            int(board[0][i] + board[1][i] + board[2][i]) == 15)
            return true;
    }
    // Check diagonals
    if (int(board[0][0] + board[1][1] + board[2][2]) == 15 ||
        int(board[0][2] + board[1][1] + board[2][0]) == 15)
        return true;
    return false;
}

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
bool X_O_Num_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}
bool X_O_Num_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}
Player<char>* XO_Num_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Numerical XO Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;

    return new Player<char>(name, symbol, type);
}
