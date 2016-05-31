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
		posOld[i] = posNew[i];
		velOld[i] = velNew[i];
		accelOld[i] = accelNew[i];
		
		this->posNew[i] = this->posOld[i] + this->velNew[i] * deltaTime;
		this->velNew[i] = this->velOld[i] + this->accelNew[i] * deltaTime;
		this->accelNew[i] = (1 / mass) * force[i];
	}
}


Physics::~Physics()
{
}
