//--------------------------------------- IMPLEMENTATION

#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include "Connect4_Classes.h"

using namespace std;

//--------------------------------------- X_O_Board Implementation

Connect4_Board::Connect4_Board() : Board(6, 7) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Connect4_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0) && (x == 5 || (x<5 && board[x+1][y] != blank_symbol))) {

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

bool Connect4_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c,char d) {
        return a == b && b == c && c==d && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            if (all_equal(board[0+i][j], board[1 + i][j], board[2 + i][j], board[3 + i][j]) && board[i][j] == sym) {
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
            if (all_equal(board[i][j + 3], board[i + 1][j+ 2], board[i + 2][j + 1], board[i + 3][j]) && board[i][j + 3] == sym) {
                return true;
            }
            if (all_equal(board[i][3-j], board[i][4-j], board[i][5-j], board[i][6-j]) && board[i][3-j] == sym) {
                return true;
            }
        }
    }

    return false;
}

bool Connect4_Board::is_draw(Player<char>* player) {
    return (n_moves == 42 && !is_win(player));
}

bool Connect4_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

Connect4_UI::Connect4_UI() : UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}

Player<char>* Connect4_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

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
