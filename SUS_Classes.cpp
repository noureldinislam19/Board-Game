#include <iostream>
#include <iomanip>
#include <cctype> 
#include <queue>
#include <utility>
#include "SUS_Classes.h"

using namespace std;

int SUS_Board::check(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    if (a.first==0&&a.second==0 && b.first==0&& b.second==1 && c.first==0&&c.second==2) {
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


}
bool taken[9]{};


SUS_Board::SUS_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

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
bool SUS_Board::is_lose(Player<char>* player) {
    if (n_moves == 9 && sus_count[1] < sus_count[0]) {
        return true;
    }
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    if (n_moves == 9 && sus_count[0] == sus_count[1]) {
        return true;
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return (n_moves==9);
}

//--------------------------------------- SUS_UI Implementation

SUS_UI::SUS_UI() : UI<char>("Welcome to our SUS Game", 3) {}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

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