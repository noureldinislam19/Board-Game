#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr

#include "BoardGame_Classes.h"
#include "SUS_Classes.h"
using namespace std;


int main() {
    
        srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

        // Create an instance of the specific UI for X-O using a pointer 
        UI<char>* game_ui = new SUS_UI();

        // Create the game board. For X-O, this is an X_O_Board.
        Board<char>* sus_board = new SUS_Board();

        // Use the UI to set up the players for the game.
        // The UI returns a dynamically allocated array of Player pointers.
        Player<char>** players = game_ui->setup_players();
       

        // Create the game manager with the board and the array of players.
        GameManager<char> sus_game(sus_board, players, game_ui);

        // Run the game loop.
        sus_game.run();

        // --- Cleanup ---
        // Delete the dynamically allocated board object.
        delete sus_board;

        // Delete the individual player objects.
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        // Delete the dynamically allocated array of player pointers itself.
        delete[] players;

        return 0;
}

// =====================================================================
