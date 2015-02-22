#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <thread>
using std::thread;
#include "JohanEngine\Renderer.h"
#include "JohanEngine\Camera.h"
#include "JohanEngine\Dialogs.h"
#include "JohanEngine\Lights.h"
#include "JohanEngine\Scene.h"
#include "Particle.h"

class Simulator {
	Window* window;
	Label* logbox;
	vector<Particle*> particles;
	vector<thread> workers;
	void ComputeGravityValues(unsigned int from,unsigned int to);
	public:
		Simulator();
		~Simulator();
		void CreateLogBox();
		void CreateLight();
		void AddSolarSystem();
		void OnUpdateTime(double dt);
		void AddParticle(const string& name,const string& objectname,const double3& position,const double3& velocity,double mass,double charge);
		void DrawLabels();
		void AddParticleGrid(int gridsize);
};

#endif
