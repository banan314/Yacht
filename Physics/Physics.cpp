#include "Physics.h"


Physics::Physics()
{
	for (int i = 0; i < 3; i++)
	{
		deltaPos[i] = 0.0;
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
		velOld[i] = velNew[i];
		accelOld[i] = accelNew[i];
		
		deltaPos[i] = this->velNew[i] * deltaTime;
		this->velNew[i] = this->velOld[i] + this->accelNew[i] * deltaTime;
		this->accelNew[i] = (1 / mass) * force[i];
	}
}


Physics::~Physics()
{
}
