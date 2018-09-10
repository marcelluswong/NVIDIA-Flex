
class RigidBody : public Scene
{

public:
	RigidBody(const char* name) : Scene(name) {}

	virtual void Initialize()
	{

		g_numExtraParticles = 30;

		// create initial cell shapes
		position = Vec3(1.0f, 1.0f, 1.0f);
		invMass = 1.0f;
		velocity = 0.0f;
		phase = NvFlexMakePhase(0, 0);
		particleSpacing = 0.15f;
		numSprings = 0;

		// particle 1
		g_buffers->positions.push_back(Vec4(position.x, position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);

		// particle 2
		g_buffers->positions.push_back(Vec4(position.x + particleSpacing, position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);

		// particle 3
		g_buffers->positions.push_back(Vec4(position.x + (2 * particleSpacing), position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);

		// particle 4
		g_buffers->positions.push_back(Vec4(position.x + (3 * particleSpacing), position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);

		// particle 5
		g_buffers->positions.push_back(Vec4(position.x + (4 * particleSpacing), position.y, position.z, invMass));
		g_buffers->velocities.push_back(velocity);
		g_buffers->phases.push_back(phase);


		CreateSpring(0, 1, 1.0f); numSprings++;
		CreateSpring(0, 2, 1.0f); numSprings++;
		CreateSpring(0, 3, 1.0f); numSprings++;
		CreateSpring(1, 2, 1.0f); numSprings++;
		CreateSpring(1, 3, 1.0f); numSprings++;
		CreateSpring(1, 4, 1.0f); numSprings++;
		CreateSpring(2, 3, 1.0f); numSprings++;
		CreateSpring(2, 4, 1.0f); numSprings++;
		CreateSpring(3, 4, 1.0f); numSprings++;

		for (int i = 0; i < 25; ++i) {
			CreateSpring(0, 1, 0.0f);
		}

		g_params.gravity[1] *= 0.0f;
		g_drawSprings = true;

		mFrame = 0;

	}





	float particleSpacing;
	int mFrame;
	int phase;
	Vec3 position;
	float invMass;
	float velocity;
	int numSprings;

};