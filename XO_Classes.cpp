#include <iostream>
#include <iomanip>
#include <cctype> 
#include <queue>
#include <utility>
#include "XO_Classes.h"

using namespace std;

// REMOVE the global queue from here.
// queue<pair<int,int>> movelist; <--- DELETED

//--------------------------------------- X_O_Board Implementation

X_O_Board::X_O_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

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
    // Infinity Tic Tac Toe usually doesn't have a draw state
    // unless you want to implement a turn limit.
    return false;
}

bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

XO_UI::XO_UI() : UI<char>("Welcome to FCAI Infinity X-O Game", 3) {}

Player<char>* XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }

    // REMOVE THIS LINE:
    // movelist.push({x,y}); 
    // We do NOT push to the queue here. We wait for the Board to validate it.

    return new Move<char>(x, y, player->get_symbol());
}