#include "Physics.h"


Physics::Physics()
{
	for (int i = 0; i < 3; i++)
	{
		posNew[i] = 0.0;
	}
	for (int i = 0; i < 3; i++)
	{
		velNew[i] = 0.0;
	}
	for (int i = 0; i < 3; i++)
	{
		accelNew[i] = 0.0;
	}
}

void Physics::computeNew(float mass, float deltaTime)
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
