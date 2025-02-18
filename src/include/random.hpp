#ifndef _RANDOM__HPP
#define _RANDOM__HPP

#include <cassert>
#include <chrono>
#include <random>

static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// gives a random integer in range [lo, hi]
inline int randint(int lo, int hi) {
  assert(lo <= hi);
  return std::uniform_int_distribution<int>(lo, hi)(rng);
}

// gives a random real number in range [lo, hi)
inline double randreal(double lo, double hi) {
  assert(lo <= hi);
  return std::uniform_real_distribution<double>(lo, hi)(rng);
}

#endif  // _RANDOM__HPP