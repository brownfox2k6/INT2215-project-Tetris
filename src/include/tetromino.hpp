#ifndef tetromino_hpp_
#define tetromino_hpp_

#include <cassert>
#include <utils.hpp>
#include <vector>

class Tetromino {
private:
public:
  TextureType shape;
  std::vector<std::pair<int, int>> pos;
  Tetromino();
  Tetromino(TextureType _shape);

  // initialize its position on the board (at the start)
  void initPos();

  // delete its shape (by making it FREE)
  void erase();

  // give it a random shape
  void initRandom();

  // check if position (x, y) on board has been occupied by this tetromino
  bool isOccupying(int x, int y) const;
};

#endif  // tetromino_hpp_