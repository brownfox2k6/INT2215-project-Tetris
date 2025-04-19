#include <cassert>
#include <random.hpp>


int randInt(int lo, int hi) {
  assert(lo <= hi);
  return std::uniform_int_distribution<int>(lo, hi)(rng);
}

float randReal(float lo, float hi) {
  assert(lo < hi);
  return std::uniform_real_distribution<float>(lo, hi)(rng);
}