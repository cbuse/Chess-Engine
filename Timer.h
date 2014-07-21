// Zugzwang Chess Engine - stage 2

#include <sys/timeb.h>
#include <stdio.h>

class Timer{
	
	int startTime, stopTime, currentTime;
	
	struct timeb startSysTime, stopSysTime, currentSysTime;
	bool isRunning;
	
	Timer();
	void startTimer();
	void stopTimer();
	void resetTimer();
	void displayTime();
	
};
