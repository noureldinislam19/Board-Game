#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"


using namespace std;

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

#endif // SUS_CLASSES_H
