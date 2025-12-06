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

XO_UI::XO_UI() : UI<char>("Welcome to FCAI Infinty X-O Game", 3) {}

XO_UI4::XO_UI4() :UI<char>("Welcome to FCAI 4x4 X-O Game", 3) {}

XO_Num_UI::XO_Num_UI() :UI<char>("Welcome to FCAI Numerical X-O Game", 3) {}

Pyramid_XO_UI::Pyramid_XO_UI() :UI<char>("Welcome to FCAI Pyramid X-O Game", 3) {}

XO_UI_WORD::XO_UI_WORD() : UI<char>("Welcome to FCAI WORD X-O Game", 3) {}

XO_UI_5::XO_UI_5() : UI<char>("Welcome to FCAI 5x5 X-O Game", 3) {}

Memo_XO_UI::Memo_XO_UI() : UI<char>("Welcome to FCAI X-O Game by Dr El-Ramly", 3) {}

obstacles_XO_UI::obstacles_XO_UI() : UI<char>("Welcome to Obstacle Tic Tac Toe!", 3) {}

Ultimate_X_O_UI::Ultimate_X_O_UI() : UI<char>("Welcome to Ultimate Tic Tac Toe!", 3) {}

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

//--------------------------------------- 4X4_X_O_Board Implementation

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
	char direction;

    if (player->get_type() == PlayerType::HUMAN) {
		cout << "It's your turn " << player->get_name();
        cout << "\nEnter your cell for 4x4 and where to move (row col 0 to 3) , (L,R,U,D) : ";
        cin >> x >> y >> direction;
        
    }
    else {
        // computer random move
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();\

                    char dirs[4] = {'L','R','U','D'};
        direction = dirs[rand() % 4];
    }

	return new DirectedMove(x, y, player->get_symbol(), direction);
}

bool X_O_Board4::is_draw(Player<char>* player) {
    return false;
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

//--------------------------------------- Word_X_O_Board Implementation


X_O_Board_WORD::X_O_Board_WORD() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

Player<char>* XO_UI_WORD::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

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
    string e ;
    string t ;
    e.push_back(board[0][0]);
    e.push_back(board[1][1]);
    e.push_back(board[2][2]);
    t.push_back(board[0][2]);
    t.push_back(board[1][1]);
    t.push_back(board[2][0]);
    if (s.find(e) != s.end() || s.find(t) != s.end()){
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

bool X_O_Board_WORD::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool X_O_Board_WORD::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


//--------------------------------------- Connect4 Implementation

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

bool Connect4_Board::is_draw(Player<char>* player) {
    return (n_moves == 42 && !is_win(player));
}

bool Connect4_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

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

//--------------------------------------- Memo_XO_Classes.cpp

Memo_X_O_Board::Memo_X_O_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    revealed = vector<vector<char>>(3, vector<char>(3, blank_symbol));
}
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
bool Memo_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}
bool Memo_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Player<char>* Memo_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

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
bool Diamond_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves >= 25 && !is_win(player));
}

bool Diamond_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}
Diamond_X_O_UI::Diamond_X_O_UI() : UI<char>("Welcome to Diamond 7x7 Tic-Tac-Toe", 3) {}

Player<char>* Diamond_X_O_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Diamond Player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

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

Player<char>* obstacles_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Obstacle Player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* obstacles_XO_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "Enter move (row col 0 to 5): ";
        cin >> x >> y;
    }
    else {
        x = rand() % 6;
        y = rand() % 6;
    }
    return new Move<char>(x, y, player->get_symbol());
}

obstacles_X_O_Board::obstacles_X_O_Board() : Board(6, 6) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    }

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
                board[rand() % 6][rand() % 6] = '#';
                board[rand() % 6][rand() % 6] = '#';
            }
        }
    }
        return true;
    }

bool obstacles_X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns - 2; ++j) {
            if (all_equal(board[i][j], board[i][j + 1], board[i][j + 2]) && board[i][j] == sym)
                return true;
        }
    }
    for (int j = 0; j < columns; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            if (all_equal(board[i][j], board[i + 1][j], board[i + 2][j]) && board[i][j] == sym)
                return true;
        }
    }
	// Check diagonals
    for (int i = 0; i < rows - 2; ++i) {
        for (int j = 0; j < columns - 2; ++j) {
            if (all_equal(board[i][j], board[i + 1][j + 1], board[i + 2][j + 2]) && board[i][j] == sym)
                return true;
        }
    }
    for (int i = 0; i < rows - 2; ++i) {
        for (int j = 2; j < columns; ++j) {
            if (all_equal(board[i][j], board[i + 1][j - 1], board[i + 2][j - 2]) && board[i][j] == sym)
                return true;
        }
    }
    
    return false;
}
bool obstacles_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 36 && !is_win(player));
}
bool obstacles_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ------------------------------ 5x5 X-O board 


Player<char>* XO_UI_5::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

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

X_O_Board_5::X_O_Board_5() : Board(5, 5) {
    for (int i = 0;i < rows;++i)
        for (int j = 0;j < columns;++j)
            board[i][j] = blank_symbol;
    n_moves = 0;
}

bool X_O_Board_5::update_board(Move<char>* move) {
    int x = move->get_x(), y = move->get_y();
    char mark = toupper(move->get_symbol());
    if (x < 0 || x >= rows || y < 0 || y >= columns || n_moves == 24) return false;
    if (board[x][y] != blank_symbol) return false;
    board[x][y] = mark;
    ++n_moves;
    return true;
}

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

bool X_O_Board_5::is_win(Player<char>* player) {
    if (n_moves < 24) return false;

    int xCount = count_three_in_row('X');
    int oCount = count_three_in_row('O');

    cout << xCount << " " << oCount<<'\n';

    if (xCount > oCount)   return player->get_symbol() == 'X';
    if (oCount > xCount)   return player->get_symbol() == 'O';

    return false; 
}

bool X_O_Board_5::is_lose(Player<char>* player) {
    if (n_moves < 24) return false;

    int xCount = count_three_in_row('X');
    int oCount = count_three_in_row('O');

    cout << xCount << " " << oCount<<'\n';

    if (xCount > oCount)   return player->get_symbol() == 'O';
    if (oCount > xCount)   return player->get_symbol() == 'X';

    return false;
}

bool X_O_Board_5::is_draw(Player<char>* player) {
    if (n_moves < 24) return false;
        char me = toupper(player->get_symbol());
        char opp = (me == 'X') ? 'O' : 'X';
        cout << count_three_in_row(opp) << " " << count_three_in_row(me) << endl;
        return count_three_in_row(me) == count_three_in_row(opp);
    
}

bool X_O_Board_5::game_is_over(Player<char>* player) {
    return n_moves >= 24;
}

//--------------------------------------------------- Ultimate_X_O_Board Implementation
Ultimate_X_O_Board::Ultimate_X_O_Board() : Board(9, 9) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}
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
        return true;
    }
    return false;
}
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
                for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                        if (board[r + i][c + j] == blank_symbol) {
							board[r + i][c + j] = '#';
                        }
                main_board[bi][bj] = sym;
            }
        }
    }

    // ---- PHASE 2: Check win in the meta-board (3x3) ----

    // Rows
    for (int i = 0; i < 3; i++)
        if (all_equal(main_board[i][0], main_board[i][1], main_board[i][2]))
            return true;

    // Columns
    for (int j = 0; j < 3; j++)
        if (all_equal(main_board[0][j], main_board[1][j], main_board[2][j]))
            return true;

    // Diagonals
    if (all_equal(main_board[0][0], main_board[1][1], main_board[2][2]))
        return true;

    if (all_equal(main_board[0][2], main_board[1][1], main_board[2][0]))
        return true;

    return false;
}

bool Ultimate_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 81 && !is_win(player));
}
bool Ultimate_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Player<char>* Ultimate_X_O_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating Ultimate X-O Player: " << name
        << " (" << symbol << ") - "
        << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << endl;
    return new Player<char>(name, symbol, type);
}

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
