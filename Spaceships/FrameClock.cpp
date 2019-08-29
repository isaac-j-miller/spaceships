#include "FrameClock.h"
#include <iostream>
unsigned long long int FrameClock::master = 0;
void FrameClock::Init(long long unsigned int val) {
	master = val;
}
void FrameClock::updateTime(long long unsigned int time) {
	master = time;
	//std::cout << master << std::endl;
}
void FrameClock::restart() {
	recentRestart = master;
}
long long unsigned int FrameClock::getTime() {
	//std::cout << master - recentRestart << std::endl;
	return master - recentRestart;
}
