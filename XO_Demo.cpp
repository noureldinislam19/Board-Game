/**
 * @file XO_Demo.cpp
 * @brief Entry point for the FCAI X-O (Tic-Tac-Toe) game.
 *
 * This file initializes the X-O game by creating the user interface, board, and players.
 * It then launches the game loop via the GameManager class.
 * All dynamically allocated objects are properly deleted at the end.
 */

#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
using namespace std;

/**
 * @brief Main function to run the X-O game.
 *
 * This function orchestrates the game by:
 * - Initializing the random number generator
 * - Creating the X-O specific UI and board
 * - Setting up players using the UI
 * - Running the main game loop through the GameManager
 * - Performing cleanup of dynamically allocated memory
 *
 * @return int Returns 0 on successful execution.
 */
void menu()
{
    cout << "Choose your option: \n";
    cout << " 1) SUS\n";
    cout << " 2) Four-in-a-row\n";
    cout << " 3) 5 x 5 Tic Tac Toe\n";
    cout << " 4) Word Tic-tac-toe\n";
    cout << " 5) Misère Tic Tac Toe\n";
    cout << " 6) Diamond Tic-Tac-Toe\n";
    cout << " 7) 4 x 4 Tic-Tac-Toe\n";
    cout << " 8) Pyramid Tic-Tac-Toe\n";
    cout << " 9) Numerical Tic-Tac-Toe\n";
    cout << "10) Obstacles Tic-Tac-Toe\n";
    cout << "11) Infinity Tic-Tac-Toe\n";
    cout << "12) Ultimate Tic Tac Toe\n";
    cout << "13) Memory Tic-Tac-Toe\n";
}
int main() {
    int x ;
    menu () ;
    cin >> x ;
    if ( x == 11 ) {
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

    // Create an instance of the specific UI for X-O using a pointer 
    UI<char>* game_ui = new XO_UI();

    // Create the game board. For X-O, this is an X_O_Board.
    Board<char>* xo_board = new X_O_Board();

    // Use the UI to set up the players for the game.
    // The UI returns a dynamically allocated array of Player pointers.
    Player<char>** players = game_ui->setup_players();

    // Create the game manager with the board and the array of players.
    GameManager<char> x_o_game(xo_board, players, game_ui);

    // Run the game loop.
    x_o_game.run();

    // --- Cleanup ---
    // Delete the dynamically allocated board object.
    delete xo_board;

    // Delete the individual player objects.
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    // Delete the dynamically allocated array of player pointers itself.
    delete[] players;

    return 0;
    }// Exit successfully
}

// =====================================================================
