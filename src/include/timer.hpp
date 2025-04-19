#ifndef timer_hpp_
#define timer_hpp_

#include <SDL3/SDL_timer.h>

class Timer {
private:
  Uint64 startTicks, pausedTicks, lastLap;
  bool isPaused, isStarted;
public:
  Timer();
  void start();
  void reset();
  void update();
  void pause();
  void resume();
  void lap();
  Uint64 distanceFromLap();
  Uint64 getTicks();
};

#endif // timer_hpp_