#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <Arduino.h>
/***********************************************************/
/* StopWatch Variables for Monitoring Time - DO NOT CHANGE */
/***********************************************************/
#define RESET   0
#define RUNNING 1
#define STOPPED 2

#define DEFAULTPERIOD 10 // 1000 = 1 Second

class StopWatch
{
  public:
    StopWatch();
    uint32_t elapsed();
    void stopWatchStart();
    void stopWatchReset();
    uint32_t stopWatchValue();
    void stopWatchStop();

  private:
    uint8_t stopWatchState = STOPPED;
    uint32_t swStartTime = 0UL;
    uint32_t swStopTime = 0UL;
};
#endif // STOPWATCH_H
