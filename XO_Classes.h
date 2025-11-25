#include "BoardGame_Classes.h"
#include <queue> // Include queue here
#include <utility> // For pair
#include <map>

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
    char blank_symbol = '.';
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