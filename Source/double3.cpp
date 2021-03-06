#include "double3.h"

double3::double3() {
}
double3::double3(double x) {
	this->x = x;
	this->y = x;
	this->z = x;
}
double3::double3(double x,double y,double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double double3::Length() {
	return sqrt(x*x + y*y + z*z);
}

float3 double3::ToFloat3() {
	return float3(x,y,z);
}

double3& double3::operator += (const double3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

double3& double3::operator -= (const double3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

double3 double3::operator + () const {
	return *this;
}

double3 double3::operator - () const {
	return double3(-x, -y, -z);
}

double3 double3::operator + (const double3& v) const {
	return double3(x + v.x, y + v.y, z + v.z);
}

double3 double3::operator - (const double3& v) const {
	return double3(x - v.x, y - v.y, z - v.z);
}

double3 double3::operator * (double f) const {
	return double3(x * f, y * f, z * f);
}

double3 double3::operator / (double f) const {
	return double3(x / f, y / f, z / f);
}
