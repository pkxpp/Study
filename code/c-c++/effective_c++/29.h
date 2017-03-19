#include <iostream>
#include <memory>

using namespace std;

class Point{
public:
	Point(int x, int y);
	
	void setX(int newVal);
	void setY(int newVal);
};

struct RectData{
	Point ulhc;
	Point lrhc;
};

class Rectangle{
public:
	Rectangle(Point&, Point&);
	Point& upperLeft() const {return pData->ulhc;}
	Point& lowerRight() const {return pData->lrhc;}
private:
	std::auto_ptr<RectData> pData;
};

