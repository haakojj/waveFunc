#include "libs.h"
#include "myClock.h"

MyClock::MyClock()
{
	lastTimePoint = Time::now();
}

MyClock::~MyClock()
{
}

float MyClock::getDeltaTime() {
	timePoint currentTimePoint = Time::now();
	dSec fs = currentTimePoint - lastTimePoint;
	lastTimePoint = currentTimePoint;
	return fs.count();
}