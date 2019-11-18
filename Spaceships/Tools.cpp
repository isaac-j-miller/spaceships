#include "Tools.h"
#include <SFML/Graphics.hpp>
#include <cmath>

levelInfo generateLevelInfo(int level) {
	levelInfo out;
	out.patrols = level/2 + 2;
	out.fighters = level/2 - 1;
	out.cruisers = level/3 - 2;
	out.carriers = level/4 - 1;
	out.aggressiveness = level/5 + 3;
	out.sluggishness = 10 - level/2;
	if (out.patrols < 0) {
		out.patrols = 0;
	}
	if (out.fighters < 0) {
		out.fighters = 0;
	}
	if (out.cruisers < 0) {
		out.cruisers = 0;
	}
	if (out.carriers < 0) {
		out.carriers = 0;
	}
	if (out.carriers > 1) {
		out.carriers = 1;
	}
	if (level == BLACKHOLE_LEVEL) {
		out.blackHoles = 1;
	}
	else {
		out.blackHoles = 0;
	}
	return out;
}

//overload operators:
point point::operator+(const point& a) const{
	point out = { a.x + x,a.y + y };
	return out;
}
point point::operator-(const point& a) const {
	point out = {  x-a.x, y -a.y};
	return out;
}
point point::operator*(const point& a) const {
	point out = { x * a.x, y * a.y };
	return out;
}
point point::operator*(const float& a) const {
	point out = { x * a, y * a };
	return out;
}
bool point::operator==(const point& a)const {
	bool out = (x == a.x && y == a.y);
	return out;
}

line_segment line_segment::operator+(const line_segment& a)const {
	line_segment out = { a.i + i, a.f + f };
	return out;
}
line_segment line_segment::operator-(const line_segment& a)const {
	line_segment out = { i - a.i,  f -a.f};
	return out;
}
line_segment line_segment::operator+(const point& a)const {
	line_segment out = { a + i, a + f };
	return out;
}
line_segment line_segment::operator-(const point& a)const {
	line_segment out = {  i-a, f -a};
	return out;
}
line_segment line_segment::operator*(const point& a)const {
	line_segment out = { i * a, f * a };
	return out;
}
bool line_segment::operator==(const line_segment& a)const {
	bool out = (i==a.i && f == a.f);
	return out;
}

box box::operator+(const box& a)const {
	box out = { a.topLeft + topLeft, a.bottomLeft + bottomLeft, a.bottomRight + bottomRight, a.topRight + topRight };
	return out;
}
box box::operator-(const box& a)const {
	box out = { a.topLeft - topLeft, a.bottomLeft - bottomLeft, a.bottomRight - bottomRight, a.topRight - topRight };
	return out;
}
box box::operator+(const point& a)const {
	box out = { a + topLeft, a + bottomLeft, a + bottomRight, a + topRight };
	return out;
}
box box::operator-(const point& a)const {
	box out = {topLeft-a, bottomLeft-a, bottomRight-a, topRight-a };
	return out;
}
box box::operator*(const point& a)const {
	box out = { topLeft * a, bottomLeft * a, bottomRight * a, topRight * a };
	return out;
}
box box::operator*(const float& a)const {
	box out = { topLeft * a, bottomLeft * a, bottomRight * a, topRight * a };
	return out;
}
bool box::operator==(const box& a)const {
	bool out = (topLeft == a.topLeft && bottomLeft == a.bottomLeft && bottomRight == a.bottomRight && topRight == a.topRight);
	return out;
}

float getSign(const float& f) {
	return (f < 0) ? -1 : 1;
}

float magnitude(const point& a) {
	return hypotf(a.x, a.y);
	//return sqrt(pow(a.x, 2) + pow(a.y, 2));
}
float vectToAngle(const point& a) {
	//float mag = magnitude(a);
	//point normV = { a.x / mag, a.y / mag };
	float angle =abs(atan(a.y/a.x));
	
	if (a.x >= 0 && a.y >= 0) {
		//std::cout << "q1" << std::endl;
		return angle;
	}
	else if (a.x >= 0 && a.y < 0) {
		//std::cout << "q4" << std::endl;
		return M_PI*2 - angle;
	}
	else if (a.x < 0 && a.y >= 0) {
		//std::cout << "q2" << std::endl;
		return M_PI - angle;
	}
	else { // a.x <0 && a.y < 0
		//std::cout << "q3" << std::endl;
		return angle + M_PI;
	}
	return angle;
}
std::ostream& operator<<(std::ostream& os, const point& a) {
	return os << '(' << a.x << ',' << a.y << ')';
}
std::ostream& operator<<(std::ostream& os, const line_segment& a) {
	return os << "i: " << a.i << ", f: " << a.f;
}
std::ostream& operator<<(std::ostream& os, const box& a) {
	return os << "tl: " << a.topLeft << ", bl: " << a.bottomLeft << ", br: " << a.bottomRight << ", tr: " << a.topRight;
}
float round_down(const float& f) {
	if (abs(f) <= FP_0) {
		return 0.00000000000000000000000;
	}
	else {
		return f;
	}
}
point round_down(const point& f) {
	return { round_down(f.x), round_down(f.y) };
}
point getComponents(const float& r) {
	point tempR = { cos(r) ,sin(r) };
	float mag = magnitude(tempR);
	tempR.x /= mag;
	tempR.y /= mag;
	//std::cout << "components of angle " << r*180/M_PI << " degrees are " << tempR << std::endl;
	return tempR;
}

point rotate(const point& init, const float&r) {
	//std::cout << "rotating point " << init << " " << r * 180 / M_PI << " degrees" << std::endl;
	float cosine = cos(r);
	float sine = sin(r);
	sf::Transform t =sf::Transform(cosine, -sine, 0, sine, cosine, 0, 0, 0, 1);
	sf::Vector2f output = sf::Vector2f(init.x,init.y);
	output = t.transformPoint(output);
	return { output.x, output.y };
	//std::cout << "cos(r), sin(r) = " << cosine << ',' << sine << std::endl;
	//point out = { init.x * cosine - init.y * sine, init.x * sine + init.y * cosine };
	//std::cout << "output position is " << out << std::endl;
	//return out;
}
box rotate(const box& init, const float& r) {
	//std::cout << "rotating " << init << std::endl;
	return { rotate(init.topLeft, r), rotate(init.bottomLeft, r), rotate(init.bottomRight, r), rotate(init.topRight, r) };
}
bool boxOverlap(const box& b1,const box& b2) { // need to update so that it works for non-rectangles * currently not in use
	point points[] = { b2.topLeft , b2.bottomLeft , b2.bottomRight , b2.topRight };
	for (auto p : points) {
		if (pointInBox(p, b1)) {
			return true;
		}
	}
	return false;
}

box inflate(const box& a, const float& factor) {
	point avg = averagePosition(a);
	box tempBox = a - avg;
	tempBox = tempBox * factor;
	tempBox = tempBox + avg;
	return tempBox;
}

bool pointInBox(const point& a, const box& b2) {
	const float infty = 10E4;
	const point endPoint = { infty, 0 };
	const line_segment top = { b2.topLeft, b2.topRight };
	const line_segment bottom = { b2.bottomLeft, b2.bottomRight };
	const line_segment left = { b2.topLeft, b2.bottomLeft };
	const line_segment right = { b2.topRight, b2.bottomRight };
	const line_segment segments[4] = { top, bottom, left, right };

	const line_segment line = { a , endPoint + a};
	int intersectionCount = 0;
	for (int j = 0; j < 4; j++) {
		//std::cout << "segment is " << segments[j] << std::endl;
		//iterate through segments
		if (linesIntersect(line, segments[j])) {
			intersectionCount++;
		}
		//std::cout << i << ',' << j << "count = " << intersectionCount[i] << std::endl;
	}
	return (intersectionCount == 1 ? true : false);
}

float wrapValue(float a, const float& min, const float& max) {
	while (a>max) {
		a -= max;
	}
	while (a < min) {
		a += max;
	}
	return a;
}

bool inRange(const point& a, const point& bounds) {
	//std::cout << a << "," << bounds << std::endl;
	//std::cout << 0 << "<" << a.x<< "<" << bounds.x <<": "<< (a.x < bounds.x && a.x>0.) <<std::endl;
	//std::cout << 0 << "<" << a.y << "<" << bounds.y << ": " << (a.y < bounds.y && a.y>0.) << std::endl;
	return ((a.x < bounds.x && a.x>0.) && (a.y < bounds.y && a.y>0.));
}

point getWrapped(const point& a, const point& bounds) {
	return { wrapValue(a.x,0,bounds.x),wrapValue(a.y,0, bounds.y) };
}

point averagePosition(const box& b) {
	point average = b.topLeft + b.bottomLeft + b.bottomRight + b.topRight;
	average = average * 0.25;
	return average;
}

bool boxWithin(const box& b1,const box& b2) { // returns true if b1 is within b2 //works
	//will go through all the points in b1 and check if they are in b2
	//do this by constructing rays for each point and counting how many times the rays intersect line segments of b2
	//std::cout << "b1: " << b1 << std::endl << "b2: " << b2 << std::endl;
	//std::cout << "in box within" << std::endl;
	const float infty = 10E4;
	const point endPoint = { infty, 0 };
	int intersectionCount[] = { 0,0,0,0 };
	//segments
	const line_segment top = { b2.topLeft, b2.topRight };
	const line_segment bottom = { b2.bottomLeft, b2.bottomRight };
	const line_segment left = { b2.topLeft, b2.bottomLeft };
	const line_segment right = { b2.topRight, b2.bottomRight };
	const line_segment segments[4] = { top, bottom, left, right };

	const line_segment tl = { b1.topLeft, b1.topLeft + endPoint };
	const line_segment bl = { b1.bottomLeft, b1.bottomLeft + endPoint };
	const line_segment br = { b1.bottomRight, b1.bottomRight + endPoint };
	const line_segment tr = { b1.topRight, b1.topRight + endPoint };
	const line_segment rays[4] = { tl,bl,br,tr };
	//for each ray, if it intersects anything other than one segment, it is out of b2
	for (int i = 0; i < 4; i++) {
		//iterate through rays
		//std::cout << "ray is " << rays[i] << std::endl;
		for (int j = 0; j < 4; j++) {
			//std::cout << "segment is " << segments[j] << std::endl;
			//iterate through segments
			if (linesIntersect(rays[i], segments[j])) {
				intersectionCount[i]+=1;
			}
			//std::cout << i << ',' << j << "count = " << intersectionCount[i] << std::endl;
		}

		if (intersectionCount[i] != 1) {
			//std::cout << "exit box within" << std::endl;
			return false;
		}
	}
	//std::cout << "exit box within" << std::endl;
	return true;
}

bool linesIntersect(line_segment a, line_segment b) {
	//std::cout << "in bool li>" << std::endl;
	if (abs((a.f - a.i).x) > FP_0 && abs((b.f - b.i).x) > FP_0) { // if neither lines are vertical (avoids infs)
		point aParam = parameterize(a);
		point bParam = parameterize(b);
		if (abs(aParam.x - bParam.x) < 1E-6) {// are parallel or co-linear
			//std::cout << a << " does not intersect (case a) " << b << std::endl;
			return false;
		}
		else { // if they intersect at some point
			//find if the point they intersect at is along both lines
			point intersection = lineIntersection(a, b);
			//std::cout << "intersection at " << intersection;
			bool out = ((pointOnLineSegment(intersection, a) && pointOnLineSegment(intersection, b)) ? true : false);
			//std::cout << (!out ? "not" : "") << " on lines " << a << " and " << b << std::endl;
			return out;
		}
	}
	else if(abs((a.f - a.i).x) > FP_0 && abs((b.f - b.i).x)<=FP_0){//if line b is vertical but not line a
		point aParam = parameterize(a);
		float miny = (b.i.y > b.f.y) ? b.f.y : b.i.y;
		float maxy = (miny == b.i.y) ? b.f.y : b.i.y;
		float y = aParam.x * b.i.x + aParam.y;

		float minx = (a.i.x > a.f.x) ? a.f.x : a.i.x;
		float maxx = (minx == a.i.x) ? a.f.x : a.i.x;

		if ((miny <= y && y <=maxy)&&(minx <= b.i.x && b.i.x <=maxx)) {
			//std::cout << a << " intersects (case b vert, !a) " << b << std::endl;
			//std::cout << miny << "<=" << y << "<=" << maxy << std::endl;
			return true;
		}
		else {
			//std::cout << a << " does not intersect (case b) " << b << std::endl;
			return false;
		}
	}
	else if (abs((a.f - a.i).x) <= FP_0 && abs((b.f - b.i).x) > FP_0) {//if line a is vertical but not line b
		point bParam = parameterize(b);
		float miny = (a.i.y > a.f.y) ? a.f.y : a.i.y;
		float maxy = (miny == a.i.y) ? a.f.y : a.i.y;
		float y = bParam.x * a.i.x + bParam.y;

		float minx = (b.i.x > b.f.x) ? b.f.x : b.i.x;
		float maxx = (minx == b.i.x) ? b.f.x : b.i.x;
		if ((miny <= y && y <= maxy) && (minx <= a.i.x && a.i.x <= maxx)) {
			//std::cout << a << " intersects (case a vert, !b) " << b << std::endl;
			//std::cout << miny << "<=" << y << "<=" << maxy << std::endl;
			return true;
		}
		else {
			//std::cout << a << " does not intersect (case c) " << b << std::endl;
			return false;
		}
	}
	else { //if both are vertical
		//std::cout << a << " is vertical and parallel to " << b << std::endl;
		return false;
	}
}

point parameterize(line_segment s) {
	float m = (s.f.y - s.i.y) / (s.f.x - s.i.x);
	float b = s.i.y - m * s.i.x;
	return { m,b };
}

bool pointOnLineSegment(point a, line_segment b) {
	//std::cout << "in pols " << std::endl;
	//std::cout << "point is " << a << "line segment is " << b << std::endl;
	if (abs((b.i - b.f).x) <= FP_0) {// if b is vertical
		//std::cout << "b is vertical" << std::endl;
		float miny = (b.i.y > b.f.y) ? b.f.y : b.i.y;
		float maxy = (miny == b.i.y) ? b.f.y : b.i.y;
		if (abs((a - b.f).x) <= FP_0) {//if x lines up
			if (a.y >= miny && a.y <= maxy) {
				//std::cout << "intersection" << std::endl;
				return true;
			}
			else {
				//std::cout << " no intersection" << std::endl;
				return false;
			}
		}
		else {
			//std::cout << " no intersection" << std::endl;
			return false;
		}
	}
	else { //if b can be parameterized
		//std::cout << " b is not vertical" << std::endl;
		point bParam = parameterize(b);
		point intersection;
		float miny = (b.i.y > b.f.y) ? b.f.y : b.i.y;
		float maxy = (miny == b.i.y) ? b.f.y : b.i.y;
		float minx = (b.i.x > b.f.x) ? b.f.x : b.i.x;
		float maxx = (minx == b.i.x) ? b.f.x : b.i.x;
		intersection.x = a.x;
		intersection.y = bParam.x * intersection.x + bParam.y;
		if ((intersection.x >= minx && intersection.x <=maxx)&&(intersection.y >= miny && intersection.y <= maxy)) {
			//std::cout << " intersection" << std::endl;
			return true;
		}
		else {
			//std::cout << " no intersection" << std::endl;
			return false;
		}
	}
}

point lineIntersection(line_segment a, line_segment b) { //a and b are parameterized non-parallel lines which are proven to intersect. returns the intersection point
	//std::cout << "in li>" << std::endl;
	//std::cout << "a.i -a.f: " << (a.i - a.f) << std::endl;
	//std::cout << "b.i -b.f: " << (b.i - b.f) << std::endl;
	point intersection;
	if ((abs((a.i - a.f).x) > FP_0) && (abs((b.i - b.f).x) > FP_0)) { //if neither lines are vertical
		//std::cout << "li case a" << std::endl;
		point aParam = parameterize(a);
		point bParam = parameterize(b);
		//std::cout << "aParam: " << aParam << ", bParam: " << bParam << std::endl;
		intersection.x = (bParam.y - aParam.y) / (aParam.x - bParam.x);
		intersection.y = aParam.x * intersection.x + aParam.y;
	}
	else if ((abs((a.i - a.f).x) > FP_0) && (abs((b.i - b.f).x) <= FP_0)) {//if line b is vertical but not line a
		//std::cout << "li case b" << std::endl;
		point aParam = parameterize(a);
		intersection.x = b.i.x;
		intersection.y = aParam.x * intersection.x + aParam.y;
	}
	else{//if line a is vertical but not line b
		//std::cout << "li case c" << std::endl;
		point bParam = parameterize(b);
		intersection.x = a.i.x;
		intersection.y = bParam.x * intersection.x + bParam.y;
	}
	//std::cout << "lines " << a <<  " & " << b << " intersect at " << intersection <<  std::endl;
	return intersection;
}

point normalizeVector(const point& a) {
	if (a.x != 0 || a.y != 0) {
		return a * (1 / magnitude(a));
	}
	else {
		return a;
	}
}

bool lineIntersectBox(line_segment a, box b) {
	//std::cout << "in lib>" << std::endl;
	line_segment top, bottom, left, right;
	top = { b.topLeft, b.topRight };
	bottom = { b.bottomLeft, b.bottomRight };
	left = { b.topLeft, b.bottomLeft };
	right = { b.topRight, b.bottomRight };
	line_segment segments[] = { top, bottom, left, right };
	for (int j = 0; j < 4; j++) {
		if (linesIntersect(a, segments[j])) {
			//std::cout << "line segment "<< a << " intersects box " << b << std::endl;
			return true;
		}
	}
	//std::cout << "line segment " << a << " does not intersect box " << b << std::endl;
	
	return false;
}

point lineBoxIntersection(line_segment a, box b, float xPriority, float yPriority) { // if cPriority is (-), looks for lowest c else if (+), highest, else ignores.
	//returns point at which line intersects box. ASSUMES THAT THE LINE ACTUALLY INTERSECTS.
	//std::cout << "in lbi>"<< std::endl;
	point intersections[2];
	//if (abs((a.i - a.f).x) > FP_0) { // if a is not a vertical line
		//point aParam = parameterize(a);
	int index = 0;
	line_segment top, bottom, left, right;
	top = { b.topLeft, b.topRight };
	bottom = { b.bottomLeft, b.bottomRight };
	left = { b.topLeft, b.bottomLeft };
	right = { b.topRight, b.bottomRight };
	line_segment segments[] = { top, bottom, left, right };
		
	for (int j = 0; j < 4; j++) {
		if (linesIntersect(a, segments[j])) {
			//std::cout << " in lBI, checking case " << j << std::endl;
			intersections[index] = lineIntersection(a, segments[j]);
			index++;
		}
	}
	//}
	//else { // fix to 
	//	intersections[0] = { a.f.x, b.bottomLeft.y };
	//	intersections[1] = { a.f.x, b.topLeft.y };
	//}
	point out;
	if (intersections[0].y != intersections[1].y or yPriority == 0) { //if not same y (even if x is same)
		if (yPriority > 0) {//prioritize more positive y
			out = ((intersections[0].y > intersections[1].y )? intersections[0] : intersections[1]);
		}
		else if (yPriority < 0) {//prioritize more negative y
			out = ((intersections[0].y < intersections[1].y) ? intersections[0] : intersections[1]);
		}
	}
	else if (intersections[0].x != intersections[1].x or xPriority == 0) { //if same y and not same x
		if (xPriority > 0) {//prioritize more positive x
			out = ((intersections[0].x > intersections[1].x) ? intersections[0] : intersections[1]);
		}
		else if (xPriority < 0) {//prioritize more negative x
			out = ((intersections[0].x < intersections[1].x) ? intersections[0] : intersections[1]);
		}
	}
	else{ // if same y and same x
		out = intersections[0];
	}
	//std::cout << a << " intersects " << b << " at " << out << std::endl;
	return out;
}

float pointDistance(const point& a, const point& b) {
	if (a.x == b.x) {
		return abs(a.y - b.y);
	}
	else {
		return magnitude(a - b);
	}
}

float lineDistance(const line_segment& a) {
	return pointDistance(a.i, a.f);
}

point rayBoxIntersection(const line_segment& a, const box& b2) {
	const line_segment top = { b2.topLeft, b2.topRight };
	const line_segment bottom = { b2.bottomLeft, b2.bottomRight };
	const line_segment left = { b2.topLeft, b2.bottomLeft };
	const line_segment right = { b2.topRight, b2.bottomRight };
	const line_segment segments[4] = { top, bottom, left, right };
	line_segment finalists[2];
	//count intersections
	int intersectionCount[] = { 0,0,0,0 };
	int j = 0;
	for (int i = 0; i < 4; i++) {
		if (linesIntersect(a, segments[i]) && j <2) {
			intersectionCount[i] ++;
			finalists[j] = segments[i];
			j++;
		}
	}
	//finalists contains the two line segments which intersect the ray
	point intersections[2] = {lineIntersection(a, finalists[0]),lineIntersection(a, finalists[1])};
	float distances[2] = { pointDistance(a.i,intersections[0]),pointDistance(a.i,intersections[1]) };
	if (distances[0] > distances[1]) {
		return intersections[1];
	}
	else {
		return intersections[0];
	}
}

bool pointInRangeOfAngle(const point& home, const point& direction, const float& maxAngle, const point& target) {
	point translatedTarget = target - home;
	float lowerVectorSubtract = vectToAngle(translatedTarget) - vectToAngle(rotate(direction, -maxAngle / 2));
	float upperVectorSubtract = vectToAngle(translatedTarget) - vectToAngle(rotate(direction, maxAngle / 2));
	return lowerVectorSubtract >= 0 && upperVectorSubtract <= 0; 
}