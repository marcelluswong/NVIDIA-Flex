
#include <ctime>
#include <cstdlib>
#include <random>
#include <math.h>



float get_random1()
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(-0.2, 0.2); // rage -0.2 - 0.2
	return dis(e);
}


float get_random2()
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(-1, 1);




	if (round(dis(e)) == 0)
	{
		static std::default_random_engine e;
		static std::uniform_real_distribution<> dis(-1, 1);
		round(dis(e));
	}	
	
	return round(dis(e));
		std::cout << "My ceiling value is " << ceil(dis(e)) << '\n';



}

class RandomGrowth : public Scene
{

public:
	RandomGrowth(const char* name) : Scene(name) {}

	virtual void Initialize()
	{



		g_sceneLower = Vec3(0.0f, 0.0f, -4.0f);
		g_sceneUpper = Vec3(0.0f, 3.0f, -2.0f);


		g_numExtraParticles = 60;

		// create initial cell shapes
		position = Vec3(-1.0f, 2.0f, 0.0f);
		invMass = 1.0f;
		velocity = 0.0f;
		phase = NvFlexMakePhase(0, 0);
		particleSpacing = 0.2f; //can change this so it overlaps and grows out
		numSprings = 0;

		// particle 1
		g_buffers->positions.push_back(Vec4(position.x, position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);

		// particle 2
		g_buffers->positions.push_back(Vec4(position.x + particleSpacing, position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);



		CreateSpring(0, 1, 1.0f); numSprings++;

		for (int i = 0; i < 10000; ++i) {
			CreateSpring(0, 1, 0.0f);
		}

		g_params.gravity[1] *= 0.0f;
		g_params.radius = 0.1f;
		g_drawSprings = true;

		mFrame = 0;

	}

	virtual void Sync()
	{
		// update solver
		NvFlexSetSprings(g_solver, g_buffers->springIndices.buffer, g_buffers->springLengths.buffer, g_buffers->springStiffness.buffer, g_buffers->springLengths.size());
	}


	void Update()
	{
	

		if (mFrame >= 100 && mFrame < 150)
		{
			growCell();


		}

		++mFrame;
	}


	void growCell()
	{
		int numSimulationParticles = NvFlexGetActiveCount(g_solver);
		int endParticleIndex = numSimulationParticles - 1;


		for (int i = 0; i < 50; ++i)
		{
			a[i] = get_random1();
			bsqrd[i] = pow(0.2, 2) - pow(a[i], 2); 
			b[i] = sqrt(bsqrd[i]) * get_random2();

			g_buffers->positions[numSimulationParticles] = (Vec4(g_buffers->positions[endParticleIndex].x + a[i], g_buffers->positions[endParticleIndex].y + b[i], g_buffers->positions[endParticleIndex].z, invMass));
			g_buffers->velocities[numSimulationParticles] = (velocity);
			g_buffers->phases[numSimulationParticles] = (phase);

		}


		//spring new particle to end particle
		g_buffers->springLengths[numSprings] = (Length(Vec3(g_buffers->positions[numSimulationParticles]) - Vec3(g_buffers->positions[endParticleIndex])));
		g_buffers->springStiffness[numSprings] = (1.0f);
		g_buffers->springIndices[numSprings * 2] = (numSimulationParticles);
		g_buffers->springIndices[(numSprings * 2) + 1] = (endParticleIndex);
		numSprings++;

		//spring new particle to end-1 particle
		g_buffers->springLengths[numSprings] = (Length(Vec3(g_buffers->positions[numSimulationParticles]) - Vec3(g_buffers->positions[endParticleIndex - 1])));
		g_buffers->springStiffness[numSprings] = (1.0f);
		g_buffers->springIndices[numSprings * 2] = (numSimulationParticles);
		g_buffers->springIndices[(numSprings * 2) + 1] = (endParticleIndex - 1);
		numSprings++;

		// compact instances 
		static std::vector<Vec4> particles(g_buffers->positions.size());
		static std::vector<Vec3> velocities(g_buffers->velocities.size());
		static std::vector<int> phases(g_buffers->phases.size());
		static std::vector<int> springIndices(g_buffers->springIndices.size());
		static std::vector<float> springLengths(g_buffers->springLengths.size());
		static std::vector<float> springStiffness(g_buffers->springStiffness.size());

		for (int i = 0; i < g_buffers->positions.size(); ++i)
		{
			particles[i] = g_buffers->positions[i];
			velocities[i] = g_buffers->velocities[i];
			phases[i] = g_buffers->phases[i];
		}

		for (int j = 0; j < g_buffers->springLengths.size(); ++j)
		{
			springLengths[j] = g_buffers->springLengths[j];
			springStiffness[j] = g_buffers->springStiffness[j];
		}

		// Spring indices for previous springs plus two new springs 
		for (int j = 0; j < g_buffers->springLengths.size() * 2; ++j)
		{
			springIndices[j] = g_buffers->springIndices[j];
		}

		//// update particle buffers
		//g_buffers->positions.assign(&particles[0], particles.size());
		//g_buffers->velocities.assign(&velocities[0], velocities.size());
		//g_buffers->phases.assign(&phases[0], phases.size());
		//g_buffers->springIndices.assign(&springIndices[0], springIndices.size());
		//g_buffers->springLengths.assign(&springLengths[0], springLengths.size());
		//g_buffers->springStiffness.assign(&springStiffness[0], springStiffness.size());

		g_buffers->activeIndices.resize(numSimulationParticles + 1);
		for (int i = 0; i < numSimulationParticles + 1; ++i)
		{
			g_buffers->activeIndices[i] = i;
		}

	}




	float particleSpacing;
	int mFrame;
	int phase;
	Vec3 position;
	float invMass;
	float velocity;
	int numSprings;
	float radius = 0.3f;
	int particleCount = 4;
	float springRest = 0.0f;
	float a[10000];
	float bsqrd[10000];
	float b[10000];
	int RandParticleIndex[10000];

};