#include <iostream>
#include <chrono>

class Timer {
public:
  Timer(unsigned long interval) : interval(interval), lastRunTime(0) {}

  bool shouldRun() {
    auto currentTime = 1;

    if(currentTime - lastRunTime >= interval) {
      lastRunTime = currentTime;
      return true;
    } else {
      return false;
    }
  }

  void reset() {
    lastRunTime = 0;
  }

private:
  unsigned long interval;
  unsigned long lastRunTime;
};
