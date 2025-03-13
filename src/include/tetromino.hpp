#ifndef tetromino_hpp_
#define tetromino_hpp_

#include <cassert>
#include <utils.hpp>
#include <vector>

class Tetromino {
private:
public:
  TextureType shape;
  std::vector<std::vector<std::pair<int, int>>> iPos;
  std::vector<std::pair<int, int>> pos;
  int dir, dx, dy;
  Tetromino();
  Tetromino(TextureType _shape);

  // initialize its position on the board (at the start)
  void getInitPos();

  // delete its shape (by making it FREE)
  void erase();

  // give it a random shape
  void initRandom();

  // check if position (x, y) on board has been occupied by this tetromino
  bool isOccupying(int x, int y) const;

  // get current position on the board of this tetromino
  void updatePos();

  // get position of this tetromino at direction dir
  std::vector<std::pair<int, int>> getPos(int dir);
};

#endif  // tetromino_hpp_