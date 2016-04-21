#include "Physics.h"


Physics::Physics()
{
}

void Physics::computeNew(float mass, float force[3], float deltaTime)
{
	for (int i = 0; i < 3; i++)
	{
		this->accelNew[i] = (1 / mass) * force[i];
		this->velNew[i] = this->velOld[i] + this->accelNew[i] * deltaTime;
		this->posNew[i] = this->posOld[i] + this->velNew[i] * deltaTime;
	}
}


Physics::~Physics()
{
}
