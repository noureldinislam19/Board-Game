/**
 * @file XO_Demo.cpp
 * @brief Entry point and menu manager for all FCAI Tic-Tac-Toe game variants.
 *
 * This file:
 *  - Displays a menu for choosing a game variant
 *  - Dynamically creates UI, board, and players based on the selection
 *  - Runs the chosen game through GameManager
 *  - Cleans up all allocated memory
 *
 * Works with all X-O games: Classic, Diamond, Numerical, Pyramid, Ultimate, etc.
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "dic.h"

using namespace std;

/**
 * @brief Displays the game selection menu.
 */
void menu() {
    cout << "Choose your option:\n"
         << " 1) SUS\n"
         << " 2) Four-in-a-row\n"
         << " 3) 5 x 5 Tic Tac Toe\n"
         << " 4) Word Tic-tac-toe\n"
         << " 5) Misere Tic Tac Toe\n"
         << " 6) Diamond Tic-Tac-Toe\n"
         << " 7) 4 x 4 Tic-Tac-Toe\n"
         << " 8) Pyramid Tic-Tac-Toe\n"
         << " 9) Numerical Tic-Tac-Toe\n"
         << "10) Obstacles Tic-Tac-Toe\n"
         << "11) Infinity Tic-Tac-Toe\n"
         << "12) Ultimate Tic Tac Toe\n"
         << "13) Memory Tic-Tac-Toe\n"
         << "14) Exit\n";
}

/**
 * @brief Runs a selected game based on the user's choice.
 *
 * This function:
 *  - Creates the correct UI and Board objects
 *  - Sets up players
 *  - Runs the GameManager loop
 *  - Ensures proper cleanup after the game ends
 *
 * @param choice The user’s numerical game choice from the menu.
 */
void run_game(int choice) {

    UI<char>* game_ui = nullptr;
    Board<char>* board = nullptr;

    switch (choice) {

    case 1:  game_ui = new SUS_UI();              board = new SUS_Board();            break;
    case 2:  game_ui = new Connect4_UI();         board = new Connect4_Board();       break;
    case 3:  game_ui = new XO_UI_5();             board = new X_O_Board_5();          break;
    case 4:  game_ui = new XO_UI_WORD();          board = new X_O_Board_WORD();       break;
    case 5:  game_ui = new XO_Inverse_UI();       board = new X_O_Inverse_Board();    break;
    case 6:  game_ui = new Diamond_X_O_UI();      board = new Diamond_X_O_Board();    break;
    case 7:  game_ui = new XO_UI4();              board = new X_O_Board4();           break;
    case 8:  game_ui = new Pyramid_XO_UI();       board = new Pyramid_X_O_Board();    break;
    case 9:  game_ui = new XO_Num_UI();           board = new X_O_Num_Board();        break;
    case 10: game_ui = new obstacles_XO_UI();     board = new obstacles_X_O_Board();  break;
    case 11: game_ui = new XO_UI();               board = new X_O_Board();            break;
    case 12: game_ui = new Ultimate_X_O_UI();     board = new Ultimate_X_O_Board();   break;
    case 13: game_ui = new Memo_XO_UI();          board = new Memo_X_O_Board();       break;

    default:
        cout << "Invalid choice.\n";
        return;
    }

    // Set up players
    Player<char>** players = game_ui->setup_players();

    // Run the game
    GameManager<char> game(board, players, game_ui);
    game.run();

    // Cleanup
    delete board;
    delete players[0];
    delete players[1];
    delete[] players;
    delete game_ui;
}

/**
 * @brief Main function. Handles menu loop and replay options.
 *
 * @return int Returns 0 when the user chooses Exit.
 */
int main() {

    srand(static_cast<unsigned int>(time(0))); // Seed RNG

    int choice;
    menu();
    cin >> choice;

    while (true) {

        if (choice == 14)
            break;

        run_game(choice);

        cout << "Do you want to play again?\n"
             << "1) Yes\n"
             << "2) No\n";

        int again;
        cin >> again;

        if (again == 2)
            break;

        cout << "1) Same game\n"
             << "2) Choose another game\n";

        int mode;
        cin >> mode;

        if (mode == 2) {
            menu();
            cin >> choice;
        }
    }

    return 0;
}
