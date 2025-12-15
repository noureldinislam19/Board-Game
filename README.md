# Board Game Collection

A **C++ console-based board game collection** implementing multiple variants of Tic-Tac-Toe and other board games using **object-oriented programming principles**. This project demonstrates abstraction, inheritance, polymorphism, and encapsulation while providing playable board games with different rules and mechanics.

---

## Table of Contents

- [Project Overview](#project-overview)  
- [Features](#features)  
- [Repository Structure](#repository-structure)  
- [Games Included](#games-included)  
- [Class Descriptions](#class-descriptions)  
- [Gameplay Instructions](#gameplay-instructions)  
- [Future Improvements](#future-improvements)  

---

## Project Overview

This project provides a **flexible, extensible framework for board games**. Each game is implemented as a subclass of **generic abstract classes** for boards, moves, and user interfaces. Current games include classic Tic-Tac-Toe, Infinity Tic-Tac-Toe, Connect Four, Word Tic-Tac-Toe, Pyramid Tic-Tac-Toe, and many more.

---

## Features

- Multiple board games with unique rules and mechanics  
- Console-based interface  
- Two-player or player vs computer modes  
- Input validation and move tracking  
- Modular and extensible OOP design  
- AI or random move generation support  

---

Board-Game
│
├─ BoardGame_Classes.h # Base classes for Board, Move, Player, UI
├─ XO_Classes.cpp # Implementations of Tic-Tac-Toe variants
├─ XO_Classes.h # Header for Tic-Tac-Toe classes
├─ XO_Demo.cpp # Demo file to run the games
├─ dic.h # Helper definitions


---

## Games Included

### 1. Infinity Tic-Tac-Toe (3×3)
- **Board Type:** `X_O_Board`  
- **UI:** `XO_UI`  
- **Special Rules:** Moves automatically remove oldest marks when more than 6 moves are played. Uses a move queue.  

### 2. 4×4 Sliding Tic-Tac-Toe
- **Board Type:** `X_O_Board4`  
- **UI:** `XO_UI4`  
- **Special Rules:** Pieces can slide in four directions (L, R, U, D). First and last rows pre-filled with alternating X and O.  

### 3. Numerical Tic-Tac-Toe (Magic Sum 15)
- **Board Type:** `X_O_Num_Board`  
- **UI:** `XO_Num_UI`  
- **Special Rules:** X plays odd numbers, O plays even numbers, each number used at most once. Win if row/col/diagonal sums to 15.

### 4. SUS Game
- **Board Type:** `SUS_Board`  
- **UI:** `SUS_UI`  
- **Special Rules:** Players form “SUS” triples. Winner has more triples after 9 moves.

### 5. Misère (Inverse) Tic-Tac-Toe
- **Board Type:** `X_O_Inverse_Board`  
- **UI:** `XO_Inverse_UI`  
- **Special Rules:** Losing occurs if a player forms 3-in-a-row. Win is always false.

### 6. Pyramid Tic-Tac-Toe
- **Board Type:** `Pyramid_X_O_Board`  
- **UI:** `Pyramid_XO_UI`  
- **Special Rules:** Uses 3×5 pyramid-shaped board. Only pyramid positions are playable.

### 7. Word Tic-Tac-Toe
- **Board Type:** `X_O_Board_WORD`  
- **UI:** `XO_UI_WORD`  
- **Special Rules:** Players place letters; win if any row, column, or diagonal forms a valid word (normal or reversed).

### 8. Connect Four (6×7)
- **Board Type:** `Connect4_Board`  
- **UI:** `Connect4_UI`  
- **Special Rules:** Gravity applies; pieces fall to lowest empty cell. Win with 4 in a row.

### 9. Memory Tic-Tac-Toe
- **Board Type:** `Memo_X_O_Board`  
- **UI:** `Memo_XO_UI`  
- **Special Rules:** Board hides true symbols behind masked cells. Players must remember positions.

### 10. Diamond Tic-Tac-Toe (7×7)
- **Board Type:** `Diamond_X_O_Board`  
- **UI:** `Diamond_X_O_UI`  
- **Special Rules:** Only diamond-shaped positions playable. Win by forming intersecting 3-cell and 4-cell lines.

### 11. Obstacles Tic-Tac-Toe (6×6 + obstacles)
- **Board Type:** `obstacles_X_O_Board`  
- **UI:** `obstacles_XO_UI`  
- **Special Rules:** Some cells become obstacles. Win with 4 in a row ignoring obstacles.

### 12. 5×5 Scoring Tic-Tac-Toe
- **Board Type:** `X_O_Board_5`  
- **UI:** `XO_UI_5`  
- **Special Rules:** Counts all 3-cell lines; winner has more lines after almost full board.

### 13. Ultimate Tic-Tac-Toe (9×9)
- **Board Type:** `Ultimate_X_O_Board`  
- **UI:** `Ultimate_X_O_UI`  
- **Special Rules:** 9×9 grid divided into 3×3 sub-boards. Each sub-board outcome tracked in a meta-board.

---

## Class Descriptions

- **Board Classes** manage the state of the board, moves, win/draw conditions, and game-specific rules.  
- **UI Classes** handle player input, move validation, and optional computer moves.  

**Specialized Classes Example:**

- `DirectedMove` – adds a direction for sliding games.  
- `X_O_Num_Board` – tracks used numbers for numerical Tic-Tac-Toe.  
- `Diamond_X_O_Board` – manages diamond-shaped valid cells internally.  
- `Memo_X_O_Board` – hides the actual board for memory-based gameplay.  

---

## Gameplay Instructions

Each game has slightly different rules and move formats:

### General Steps

1. **Select a game** from the menu when the program starts.  
2. **Follow on-screen prompts** to enter moves.  
   - Classic Tic-Tac-Toe: `(row, column)`  
   - Sliding Tic-Tac-Toe: `(row, column, direction)`  
   - Numerical Tic-Tac-Toe: `(row, column, number)`  
   - Word Tic-Tac-Toe: `(row, column, letter)`  
   - Connect Four: select a column  
   - Other variants: follow the custom prompts for the specific game  
3. **Game state updates** after every move.  
4. **Win / lose / draw** is automatically detected based on game rules.  

### Tips

- Pay attention to **special rules** like obstacles, memory boards, or pyramid-shaped grids.  
- Some games have **random computer moves** if playing against AI.  
- For games like Ultimate Tic-Tac-Toe, the **sub-board to play in** may be dictated by the opponent’s previous move.  

---

## Future Improvements

1. **AI Opponents:** Implement smarter computer strategies for each variant.  
2. **Graphical Interface:** Move from console-based to graphical using **SDL** or **SFML**.  
3. **Additional Games:** Include games like **Checkers**, **Chess**, Connect Four variants, or other board games.  
4. **Enhanced Multiplayer:** Add networked multiplayer support.  
5. **Better User Feedback:** Add sound, colors, and improved input validation.  
6. **Statistics & Scoring:** Track wins, losses, and scores over multiple sessions.  

## Repository Structure

