#include "Particle.h"


Particle::Particle() {
	this->mass = 0;
	this->charge = 0;
	this->gravityforce = double3(0);
	this->position = double3(0);
	this->velocity = double3(0);
	this->acceleration = double3(0);
	this->object = NULL;
	InitializeCriticalSection(&gravitylock);
}

Particle::Particle(const string& name,const string& objectname,const double3& position,const double3& velocity,double mass,double charge) {
	this->name = name;
	this->mass = mass;
	this->charge = charge;
	this->gravityforce = double3(0);
	this->position = position;
	this->velocity = velocity;
	this->acceleration = double3(0);
	this->object = new Object(
		objectname.c_str(),
		this->position.ToFloat3(),
		float3(0,0,0),
		1);
	InitializeCriticalSection(&gravitylock);
}

Particle::~Particle() {
	if(this->object) {
		delete this->object;
		this->object = NULL;
	}
	DeleteCriticalSection(&gravitylock);
}

void Particle::OnUpdateTime(double data) {
	double3 force = gravityforce;
	
	// Set new position
	position += velocity * data;
	object->SetTranslation(position.ToFloat3());
	
	// Set new velocity
	velocity += acceleration * data;
	
	// Set new acceleration
	acceleration = force / mass;
	
	// Reset forces
	gravityforce = double3(0);
}

void Particle::AddGravity(double3 gravityforce) {
	EnterCriticalSection(&gravitylock);
	this->gravityforce += gravityforce;
	LeaveCriticalSection(&gravitylock);	
}

double3 Particle::GetPosition() {
	return this->position;
}

double3 Particle::GetVelocity() {
	return this->velocity;
}

double Particle::GetMass() {
	return this->mass;
}

string& Particle::GetName() {
	return this->name;
}

double Particle::GetRadius() {
	if(object) {
		return object->worldr;
	} else {
		return 0;
	}
}
