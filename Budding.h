#include <cstdlib>

class Budding : public Scene
{

public:
	Budding(const char* name) : Scene(name) {}

	virtual void Initialize()
	{


		g_sceneLower = Vec3(-1.0f, -1.0f, -1.0f);
		g_sceneUpper = Vec3(1.0f, 1.0f, 1.0f);


		g_numExtraParticles = 60;

		for (int i = 0; i < 10000; ++i)
		{
			r[i] = Randf(0, 0.1);
		}



		// create initial cell shapes
		position = Vec3(r[0], r[1], r[2]);
		invMass = 1.0f;
		velocity = 0.0f;
		phase = NvFlexMakePhase(0, 0);
		particleSpacing = 0.1f; //can change this so it overlaps and grows out
		numSprings = 0;

		// particle 1
		g_buffers->positions.push_back(Vec4(position.x, position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);

		// particle 2
		g_buffers->positions.push_back(Vec4(r[3], r[4], r[5], invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);


		CreateSpring(0, 1, 1.0f); numSprings++;

		for (int i = 0; i < 10000; ++i) {
			CreateSpring(0, 1, 0.5f);
		}

		g_params.gravity[1] = 0.0f;
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

		if (mFrame >= 100 && mFrame < 130)
		{
			growCell();
		}

		++mFrame;
	}

	void growCell()
	{

		int numSimulationParticles = NvFlexGetActiveCount(g_solver);
		int endParticleIndex = Rand(0, numSimulationParticles);
		int endParticleIndex2 = Rand(0, numSimulationParticles);

		for (int i = 0; i < 10000; ++i) //change i<x if need more positions. (x,y,z). 
		{
			p[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.1 - 0)));
		}


		g_buffers->positions[numSimulationParticles] = (Vec4(g_buffers->positions[endParticleIndex].x + p[0], g_buffers->positions[endParticleIndex].y + p[1], g_buffers->positions[endParticleIndex].z + p[2], invMass));
		g_buffers->velocities[numSimulationParticles] = (velocity);
		g_buffers->phases[numSimulationParticles] = (phase);

		g_buffers->springLengths[numSprings] = (Length(Vec3(g_buffers->positions[numSimulationParticles]) - Vec3(g_buffers->positions[endParticleIndex])));
		g_buffers->springStiffness[numSprings] = (1.0f);
		g_buffers->springIndices[numSprings * 2] = (numSimulationParticles);
		g_buffers->springIndices[(numSprings * 2) + 1] = (endParticleIndex);
		numSprings++;

		g_buffers->springLengths[numSprings] = (Length(Vec3(g_buffers->positions[numSimulationParticles]) - Vec3(g_buffers->positions[endParticleIndex - 1])));
		g_buffers->springStiffness[numSprings] = (1.0f);
		g_buffers->springIndices[numSprings * 2] = (numSimulationParticles);
		g_buffers->springIndices[(numSprings * 2) + 1] = (endParticleIndex2);
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
	int particleCount = 1;
	float springRest = 0.0f;
	//float r[6];
	float r[10000];
	float p[10000];

};