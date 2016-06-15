#include <math.h>

using namespace std;

// Class to represent points.
class Point {
private:
	double xval, yval;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	Point(double x = 0.0, double y = 0.0) {
		xval = x;
		yval = y;
	}

	// Extractors.
	double x() { return xval; }
	double y() { return yval; }

	// Distance to another point.  Pythagorean thm.
	double dist(Point other) {
		double xd = xval - other.xval;
		double yd = yval - other.yval;
		return sqrt(xd*xd + yd*yd);
	}
};

class Line
{
public:
	Line();
	Line(Point P1, Point P2);
	double GetA();
	double GetB();
	double calculateDistance(Point);
	bool onTheRight(Point), above(Point);
private:
	Point p1, p2;
	double A, B, C;
};

//Default constructor
Line::Line() :A(0), B(0)
{
	//left blank intentionally
}
//Point1_Object & Point2_Object were suppose to get
//Passed into here
Line::Line(Point P1, Point P2)
{
	p1 = P1;
	p2 = P2;
	double x1, x2, y1, y2;
	x1 = P1.x();
	x2 = P2.x();
	y1 = P1.y();
	y2 = P2.y();

	//derived from slope(m) intercept(b) form
	if (x1 != x2)
	{
		double m, b;
		m = (y2 - y1) / (x2 - x1);
		b = y1 - (y2 - y1)/(x2 - x1) * x1;

		A = -1;
		B = m;
		C = b;
	}
	else 
	{
		A = 0; 
		B = 1;
		C = -x1;
	}
}

//ACCESSORS
double Line::GetA()
{
	return A;
}
double Line::GetB()
{
	return B;
}

double Line::calculateDistance(Point P)
{
	double f = abs(A*P.y() + B*P.x() + C);
	return f / (sqrt(A*A + B*B));
}

bool Line::onTheRight(Point P)
{
	double linePoint;
	linePoint = A*P.y() + B*P.x() + C;
	return (linePoint*B > 0.0);
}

bool Line::above(Point P)
{
	double linePoint;
	linePoint = A*P.y() + B*P.x() + C;
	return (linePoint > 0.0);
}