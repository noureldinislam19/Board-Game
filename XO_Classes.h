#include "BoardGame_Classes.h"
#include <queue> // Include queue here
#include <utility> // For pair
#include <map>
#ifndef XO_CLASSES_H
#define XO_CLASSES_H

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
class X_O_Inverse_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    X_O_Inverse_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player) { return false; };
    bool is_lose(Player<char>*);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};
class XO_Inverse_UI : public UI<char> {
public:
    XO_Inverse_UI();
    ~XO_Inverse_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
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
#endif // XO_CLASSES_H