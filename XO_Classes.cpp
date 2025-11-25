#include <iostream>
#include <iomanip>
#include <cctype> 
#include <queue>
#include <utility>
#include "XO_Classes.h"
#include <map>

using namespace std;

XO_UI::XO_UI() : UI<char>("Welcome to FCAI Infinty X-O Game", 3) {}

XO_UI4::XO_UI4() :UI<char>("Welcome to FCAI 4x4 X-O Game", 3) {}

XO_Num_UI::XO_Num_UI() :UI<char>("Welcome to FCAI Numerical X-O Game", 3) {}

Pyramid_XO_UI::Pyramid_XO_UI() :UI<char>("Welcome to FCAI Pyramid X-O Game", 3) {}



//--------------------------------------- Infinty_X_O_Board Implementation
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
    return false;
}

bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation


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
    for (char i = '1'; i <= '9'; i++) {
        used_numbers[i] = false;
    }
}
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
            if(n_moves % 2 == 0) // X player
            {
                // Check if the number is odd
                if((mark - '0') % 2 == 0 || used_numbers[mark - '0'])
                    return false; // Invalid move
            }
            else // O player
            {
                // Check if the number is even
                if((mark - '0') % 2 != 0 || used_numbers[mark - '0'])
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

//--------------------------------------- SUS_X_O_Board Implementation

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
    return (n_moves == 9);
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

//--------------------------------------Inverse_X_O_Board Implementation
X_O_Inverse_Board::X_O_Inverse_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

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

bool X_O_Inverse_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_lose(player));
}

bool X_O_Inverse_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

XO_Inverse_UI::XO_Inverse_UI() : UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}

Player<char>* XO_Inverse_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

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

Pyramid_X_O_Board::Pyramid_X_O_Board() : Board(3, 5) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
 board[0][0] = '*'; board[0][1] = '*'; board[0][3] = '*'; board[0][4] = '*';
 board[1][0] = '*'; board[1][4] = '*';
}

bool Pyramid_X_O_Board::update_board(Move<char>* move) {
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
    for(int i = 0; i < 3; ++i) {
        if ((all_equal(board[2][i], board[2][i + 1], board[2][i + 2]) && board[2][i] == sym))
            return true;
	}
    return false;
}
bool Pyramid_X_O_Board::is_lose(Player<char>* player) {
    return false;
}
bool Pyramid_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}
bool Pyramid_X_O_Board::game_is_over(Player<char>* player) {
	return is_win(player) || is_draw(player);
}

Player<char>* Pyramid_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Pyramid X-O Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;
    return new Player<char>(name, symbol, type);
}

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
