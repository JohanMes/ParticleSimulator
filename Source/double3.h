#ifndef DOUBLE3_H
#define DOUBLE3_H

#include <cmath>
#include "JohanEngine\float3.h"

class double3 {
	public:
		double3();
		double3(double x);
		double3(double x,double y,double z);
		
		double x;
		double y;
		double z;
		
		double3& operator += (const double3& v);
		double3& operator -= (const double3& v);
		
	    double3 operator + () const;
	    double3 operator - () const;
		
	    double3 operator + (const double3& v) const;
	    double3 operator - (const double3& v) const;		
		double3 operator * (double) const;
		double3 operator / (double) const;

		double Length();
		float3 ToFloat3(); // TODO: make operator?
};

#endif
