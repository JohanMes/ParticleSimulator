#include "Simulator.h"

Simulator::Simulator() {
	// Do not create windows yet
	window = NULL;
	logbox = NULL;

	// Create reusable threads
	unsigned int threadcount = 1;//std::thread::hardware_concurrency();
	if(threadcount == 0) {
		threadcount = 1;
	}
	workers.resize(threadcount);
}

Simulator::~Simulator() {
	if(window) {
		delete window;
		window = NULL;
	}
}

void Simulator::CreateLogBox() {
	window = new Window(5,5,400,400,"ParticleSim");
	logbox = new Label(5,20,390,375,"",true);
	window->AddChild(logbox);
	window->Show(1);
}

void Simulator::CreateLight() {
	new Dirlight(
	    float3(1e2,1e2,-1e2),
	    float3(5),
	    true);
	scene->lights->SetAmbientlight(0.1);
}

// time = days
// mass = solar mass
// length = AU
void Simulator::AddSolarSystem() {
//	double M0 = 1.989e30; // solar mass
//	double T0 = 60; // minutes
//	double X0 = 1.496e11; // 1 AU

	// Sun
	double Msun = 1.989e30;
	AddParticle(
	    "Sun",
	    "WhiteParticle",
	    double3(0,0,0),
	    double3(0,0,0),
	    Msun,
	    0);

	// Planet
	double Mearth = 972e24;
	double Vearth = 27.78e3;
	AddParticle(
	    "Earth",
	    "WhiteParticle",
	    double3(1.496e11,0,0),
	    double3(0,0,Vearth),
	    Mearth,
	    0);
}

void Simulator::ComputeGravityValues(unsigned int from,unsigned int to) {
	for(unsigned int i = from; i <= to; i++) {
		for(unsigned int j = i + 1; j < particles.size(); j++) {
			// Get particles
			Particle* particle1 = particles[i];
			Particle* particle2 = particles[j];

			// Get mass parameters
			double m1 = particle1->GetMass();// * M0;
			double m2 = particle2->GetMass();// * M0;

			// Get distance parameters
			double3 p1 = particle1->GetPosition();// * X0;
			double3 p2 = particle2->GetPosition();// * X0;

			// Get distance vector
			double3 r12 = p1 - p2;
			double r12length = r12.Length();
			//	if(r12length < particle1->GetRadius() or r12length < particle2->GetRadius()) {
			//		continue; // objects are inside of each other. Skip calculations
			//	}
			if(r12length < 2) {
				continue;
			}
			double3 r12unity = r12/r12length;

			// Get force
			double G = 6.673e-11/* * pow(X0,3) * (1/M0) * (1/X0) * (1/X0)*/;
			double3 f = r12unity * (G * m1 * m2) / (r12length * r12length);

			// Set force
			particle1->AddGravity(-f);
			particle2->AddGravity(f);
		}
	}
}

void Simulator::OnUpdateTime(double dt) {
	// Divide work among workers
	unsigned int binsize = particles.size() / workers.size();
	for(unsigned int i = 0; i < workers.size(); i++) {
		// Give last bit of work to last thread
		if(i != workers.size() - 1) {
			workers[i] = thread(&Simulator::ComputeGravityValues,this,i * binsize,(i + 1) * binsize - 1);
		} else {
			workers[i] = thread(&Simulator::ComputeGravityValues,this,i * binsize,particles.size() - 1);
		}
	}
//	ComputeGravityValues(0,particles.size()-1);

	// Wait for them all to finish
	for(unsigned int i = 0; i < workers.size(); i++) {
		workers[i].join();
	}

	// Apply sum of gravity
	for(unsigned int i = 0; i < particles.size(); i++) {
		Particle* particle = particles[i];
		particle->OnUpdateTime(/*T0 * */dt);
	}

	// Set log
	if(logbox) {
		char buffer[4096];
		logbox->SetCaption("");
		for(unsigned int i = 0; i < particles.size(); i++) {
			Particle* particle = particles[i];
			snprintf(buffer,4096,
			         "Name: %s\nMass: %g\nPosition: %g,%g,%g\nVelocity: %g,%g,%g\n\n",
			         particle->GetName().c_str(),
			         particle->GetMass(),
			         particle->GetPosition().x,
			         particle->GetPosition().y,
			         particle->GetPosition().z,
			         particle->GetVelocity().x,
			         particle->GetVelocity().y,
			         particle->GetVelocity().z);
			logbox->AddCaption(buffer);
		}
	}
}

void Simulator::AddParticle(const string& name,const string& objectname,const double3& position,const double3& velocity,double mass,double charge) {
	particles.push_back(new Particle(name,objectname,position,velocity,mass,charge));
}

void Simulator::DrawLabels() {
	char buffer[4096];
	for(unsigned int i = 0; i < particles.size(); i++) {
		Particle* particle = particles[i];

		// Convert the world position to projection coordinates
		float3 worldpos = particle->GetPosition().ToFloat3();
		float3 projectionpos = worldpos.TransformCoord(camera->matViewProj);
		float2 pixelpos = renderer->ProjectionToPixels(float2(projectionpos.x,projectionpos.y));

		// Draw info line
		snprintf(buffer,4096,
		         "Name: %s\nMass: %g\nPosition: %g,%g,%g\nVelocity: %g,%g,%g\n\n",
		         particle->GetName().c_str(),
		         particle->GetMass(),
		         particle->GetPosition().x,
		         particle->GetPosition().y,
		         particle->GetPosition().z,
		         particle->GetVelocity().x,
		         particle->GetVelocity().y,
		         particle->GetVelocity().z);
		renderer->DrawTextLine(buffer,pixelpos.x,pixelpos.y);
	}
}

void Simulator::AddParticleGrid(int gridsize) {
	for(int x = -gridsize; x <= gridsize; x++) {
		for(int y = -gridsize; y <= gridsize; y++) {
			if(x == 0 and y == 0 ) {
				AddParticle(
				    "GridParticle",
				    "RedParticle.object",
				    double3(x*10,0,y*10),
				    double3(0,0,0),
				    1e11,
				    0);
			} else {
				AddParticle(
				    "GridParticle",
				    "WhiteParticle.object",
				    double3(
				        x*10,
				        0,
				        y*10),
				    double3(
				        y*1e-1,
				        0,
				        x*1e-1),
				    1e8,
				    0);
			}
		}
	}
}
