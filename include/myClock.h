#pragma once
#include "libs.h"

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::time_point<Time> timePoint;
typedef std::chrono::duration<float> dSec;

class MyClock
{
public:
	MyClock();
	~MyClock();
	float getDeltaTime(); // Return elapsed time in seconds
private:
	timePoint lastTimePoint;
};