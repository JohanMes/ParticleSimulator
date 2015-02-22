#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
using std::string;
#include "JohanEngine\Object.h"
#include "double3.h"

class Particle {
	double mass;
	double charge;
	double3 gravityforce; // temp
	double3 position; // x
	double3 velocity; // v
	double3 acceleration; // a
	Object* object;
	string name;
	CRITICAL_SECTION gravitylock;
	public:
		Particle();
		Particle(const string& name,const string& objectname,const double3& position,const double3& velocity,double mass,double charge);
		~Particle();
		void OnUpdateTime(double data);
		void AddGravity(double3 gravityforce);
		double3 GetPosition();
		double3 GetVelocity();
		double GetMass();
		string& GetName();
		double GetRadius();
};

#endif
