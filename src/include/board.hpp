#ifndef board_hpp_
#define board_hpp_

#include "SDL3/SDL_stdinc.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <cstring>
#include <deque>
#include <tetromino.hpp>
#include <unordered_map>
#include <utils.hpp>


class Board {
private:
public:
  // just last 20 rows are used to display, first 3 rows are hidden
  static const int VISIBLE_ROWS = 20;
  static const int HIDDEN_ROWS = 3;
  static const int ROWS = HIDDEN_ROWS + VISIBLE_ROWS;
  static const int COLS = 10;
  static const int HOLDS = 3;
  TextureType matrix[ROWS][COLS];
  std::deque<Tetromino> next;
  Tetromino current, ghost, hold;
  Uint64 timeCur, timePrev;
  bool isPlaying;

  Board();

  // Resets the board to an empty state
  void reset();

  // go to next state of the game
  void nextState();

  // a previous tetromino has ended its journey,
  // time to get a new tetromino to `current` from `next`
  void nextTetromino();

  // create a new tetromino and push to `next`
  void pushNewTetromino();

  // Checks if a position is within bounds
  bool isWithinBounds(int row, int col) const;

  // Checks if a position is occupied
  bool isOccupied(int row, int col) const;

  // Checks if a row is full
  bool isRowFull(int row) const;

  // Clear full rows and returns the number of cleared rows
  int clearFullRows();

  // Clears a specific row
  void clearRow(int row);

  // Shift all rows from specific start_row down by one row
  void shiftRowsDown(int start_row);

  // Checks if the game is over
  // (if any hidden rows contains any block)
  bool isGameOver() const;

  // attach board to the Renderer
  void
  attachToRenderer(std::unordered_map<TextureType, SDL_Texture *> &textures,
                   SDL_Renderer *renderer);

  // make the current tetromino one step down
  // return true if it can go down, false otherwise
  bool goDown();

  // make the current tetromino one step left
  // return true if it can go left, false otherwise
  bool goLeft();

  // make the current tetromino one step right
  // return true if it can go right, false otherwise
  bool goRight();

  // make the current tetromino to rotate 90 degrees to the left
  // return true if it can rotate, false otherwise
  bool rotateLeft();

  // make the current tetromino to rotate 90 degrees to the right
  // return true if it can rotate, false otherwise
  bool rotateRight();

  // calculate where the ghost piece is
  void getGhost();
};

#endif // board_hpp_