#pragma once
#include <iostream>
#ifndef FP_0
#define FP_0 1E-6
#endif
#ifndef BLACKHOLE_LEVEL
#define BLACKHOLE_LEVEL 3
#endif
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#ifndef TOOLS_H
#define TOOLS_H

struct levelInfo
{
	int patrols = 0;
	int fighters = 0;
	int cruisers = 0;
	int carriers = 0;
	unsigned int difficulty = 2;
	int blackHoles = 0;
};

struct point
{
	float x = 0;
	float y = 0;
	point operator+(const point& a)const;
	point operator-(const point& a)const;
	point operator*(const point& a)const;
	point operator*(const float& a)const;
	bool operator==(const point& a)const;
};

struct line_segment
{
	point i;
	point f;
	line_segment operator+(const line_segment& a)const;
	line_segment operator-(const line_segment& a)const;
	line_segment operator+(const point& a)const;
	line_segment operator-(const point& a)const;
	line_segment operator*(const point& a)const;
	bool operator==(const line_segment& a)const;
};


struct box
{
	point topLeft = { 0,0 };
	point bottomLeft = { 0,0 };
	point bottomRight = { 0,0 };
	point topRight = { 0,0 };
	box operator+(const box& a)const;
	box operator-(const box& a)const;
	box operator+(const point& a)const;
	box operator-(const point& a)const;
	box operator*(const point& a)const;
	box operator*(const float& a)const;
	bool operator==(const box& a)const;
};

levelInfo generateLevelInfo(int level);

std::ostream& operator<<(std::ostream& os, const point& a);
std::ostream& operator<<(std::ostream& os, const line_segment& a);
std::ostream& operator<<(std::ostream& os, const box& a);

bool inRange(const point& a, const point& bounds);
float wrapValue(float a, const float& min, const float& max);
point getWrapped(const point& a, const point& bounds);
point averagePosition(const box& b);
bool pointInBox(const point& a, const box& b);
float magnitude(const point& a);
float vectToAngle(const point& a);
point rotate(const point& init, const float& r);
box rotate(const box& init, const float& r);
float pointDistance(const point& a, const point& b);
bool linesIntersect(line_segment a, line_segment b);
point getTopLeft(const box&);
float round_down(const float& f);
point round_down(const point& f);
point getComponents(const float& r);
point rayBoxIntersection(const line_segment& a, const box& b2);
point lineIntersection(line_segment a, line_segment b);
float lineDistance(const line_segment& a);
point parameterize(line_segment s);
box inflate(const box& a, const float& factor);
bool pointOnLineSegment(point a, line_segment b);
point normalizeVector(const point& a);
bool boxOverlap(const box& b1,const box& b2);

bool lineIntersectBox(line_segment a, box b);

point lineBoxIntersection(line_segment a, box b, float xPriority, float yPriority);

bool boxWithin(const box& b1,const box& b2);
#endif