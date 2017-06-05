#include "Point.hpp"

void Point::Normalize(void)
{
	if(x != 0 || y != 0)
	{
		double length = Length();

		x /= length;
		y /= length;
	}
}

Point Point::Normalized() const
{
	Point normalized(x,y);

	normalized.Normalize();

	return normalized;
}

double Point::Distance(const Point& vec) const
{
	double disX = vec.x - x;
	double disY = vec.y - y;
	return sqrt(disX*disX+disY*disY);
}

double Point::DistanceSq(const Point& vec) const
{
	double disX = (vec.x - x);
	double disY = (vec.y - y);
	return disX*disX+disY*disY;
}

Point Point::VectorWithAngle(double t) const
{
	Point withAngle;
	double r = Length();
	return Polar(r,t);
}

void Point::SetAngle(double t)
{
	double r = Length();
	x = r*cos(t);
	y = r*sin(t);
}

Point Point::Rotated(double t) const
{
	double SIN = sin(t);
	double COS = cos(t);
	
	double xprime = COS*x-SIN*y;
	double yprime = SIN*x+COS*y;
	return Point(xprime,yprime);
}

void Point::Rotate(double t)
{
	double SIN = sin(t);
	double COS = cos(t);
	
	double xprime = COS*x-SIN*y;
	
	y = SIN*x+COS*y;
	x = xprime;
}

void Point::Rotate(double SIN, double COS)
{
	double xprime = COS*x-SIN*y;
	
	y = SIN*x+COS*y;
	x = xprime;
}

Point Point::Rotated(double sint, double cost) const
{
	Point a = *this;
	a.Rotate(sint, cost);
	return a;
}


void Point::SetLength(double r)
{
	Normalize();
	x *= r;
	y *= r;
}

void Point::SetLengthSq(double r2)
{
   double m = sqrt(r2/LengthSq());
	x *= m;
   y *= m;
}

void Point::SetPolar(double r, double t)
{
	x=1;
	y=1;
	SetAngle(t);
	SetLength(r);
}

Point Point::WithLength(double r) const
{
	return r*Normalized();
}

Point Point::WithLengthSq(double r2) const
{
	Point copy = *this;
	copy.SetLengthSq(r2);
   return copy;
}

Point Point::Projection(const Point& H) const
{
	Point T(x,y);
	double t = (T*H)/(H*H);
	Point proj = t*H;
	return proj;
}

Point Point::ProjectionToLine(const Point& A, const Point& B) const
{
// 	cout << "Projection to line" << endl;
	Point me = *this;
	Point meMinusA = me - A;
	Point BMinusA  = B  - A;
// 	cout << "baaaa" << endl;
	Point temp = meMinusA.Projection(BMinusA);
// 	cout << "temp: " << temp << endl;
	return temp + A;
}

Point Point::ForwardProjection(const Point& H) const
{
	if (fabs(AngleTo(H)) > pi/2)
		return Point(0,0);
	return Projection(H);
}

bool Point::IsToTheLeftOfLine(const Point& A, const Point& B) const
{
	Point H = B - A;
	Point P = (*this) - A;
	if (H.x*P.y > H.y*P.x)
		return true;
	return false;
}

bool Point::IsOnLine(const Point& A, const Point& B) const
{
	Point H = B - A;
	Point P = (*this) - A;
	return (H.x*P.y == H.y*P.x);
}

bool Point::IsToTheRightOfLine(const Point& A, const Point& B) const
{
	Point H = B - A;
	Point P = (*this) - A;
	if (H.x*P.y < H.y*P.x)
		return true;
	return false;
}


const Point& Point::operator=(const Point& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

bool Point::operator!=(const Point& vec) const
{
	return ((x != vec.x) || (y != vec.y));
}

bool Point::operator==(const Point& vec) const
{
	return ((x == vec.x) && (y == vec.y));
}

void Point::Truncate(double r)
{
	if (LengthSq() > r*r)
		SetLength(r);
}

Point Point::Truncated(double r) const
{
	if (LengthSq() > r*r)
		return WithLength(r);
	return *this;
}


std::ostream& operator<<(std::ostream& os, const Point& rhs)
{
  os << "(" << rhs.x << ", " << rhs.y << ")";

  return os;
}

Point Point::Polar(double r, double theta)
{
	return Point(r*cos(theta),r*sin(theta));
}

Point Point::RandomPoint(double maxLength)
{
	double r = random_double(0.0,maxLength);
	double theta = random_double(-pi,pi);
	return Polar(r,theta);
}

Point Point::RandomPoint(double minLength, double maxLength)
{
	double r = random_double((minLength),(maxLength));
	double theta = random_double((-pi),(pi));
	return Polar(r,theta);
}

Point Point::LocalToGlobal(const Point& origin,
								 const Point& U, 
								 const Point& V) const 
{ 
	return origin + x*U + y*V; 
}

Point Point::GlobalToLocal(const Point& origin,
								 const Point& U,
								 const Point& V) const
{
	Point L = *this - origin;
	double determinant = (U.x*V.y - U.y*V.x);
	
	//check to see if they U and V are linearly dependent
	if (fabs(determinant) != 0) return L.Projection(U);
	
	double locx = (L.x*V.y - L.y*V.x)/determinant;
	double locy = -(L.x*U.y - L.y*U.x)/determinant;
	return Point(locx,locy);
}

bool operator<(const Point& A, const Point& B)
{
	if (A.x > B.x)
		return true;
	if (A.x < B.x)
		return false;
	return (A.y >= B.y);
}

vector<Point> GenerateRandomPoints(int n, int resolution)
{
	vector<Point> result;
	
	result.reserve(n);
	
	double r = resolution;
	
	Point Centro(random_double(0.0,r), random_double(0.0,r));
	cout << "point2d("<< Centro << ")+";
	while (result.size() < n)
	{
		Point P(random_double(0.0,r), random_double(0.0,r));
		if (P.Distance(Centro) < 1000)
			result.emplace_back(P);
	}
	
	return result;
}