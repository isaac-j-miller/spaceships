#pragma once
#ifndef FRAMECLOCK_H
#define FRAMECLOCK_H
class FrameClock
{
	static long long unsigned int master;
public:
	static void updateTime(long long unsigned int time);
	void restart();
	static void Init(long long unsigned int val);
	long long unsigned int getTime();
private:
	long long unsigned int recentRestart = 0;
};

#endif