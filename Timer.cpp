// Zugzwang Chess Engine - stage 2

#include "Timer.h"

Timer::Timer(){
	startTime = 0;
	stopTime = 0;
	currentTime = 0;
	isRunning = false;
		 
}
	
	
void Timer::startTimer(){
	if(!isRunning){
		isRunning = true;
		ftime(&startSysTime);
		startTime =(int) (startSysTime.time * 1000 + startSysTime.millitm);
			
	}
	return;
}
	
void Timer::stopTimer(){
	if(isRunning){
		isRunning = false;
		ftime(&stopSysTime);
		stopTime = (int) (stopSysTime.time *1000 + stopSysTime.millitm);
			 
	}
	return;
}
	
void Timer::resetTimer(){
		
	if(isRunning){
		ftime(&startSysTime);
		startTime = (int) (startSysTime.time *1000 + startSysTime.millitm);
			
	}
	else {
		startTime = stopTime;
	}
		
	return;
}
	
void Timer::displayTime(){
	
	printf("Current time:\n");
	if (isRunning)
		{
            ftime(&currentSysTime);
            currentTime = currentSysTime.time * 1000 + currentSysTime.millitm;
            printf("%6.2f", (currentTime - startTime)/1000.0);
		}
    else
            printf("%6.2f", (stopTime - startTime)/1000.0);
    return;
		
}
