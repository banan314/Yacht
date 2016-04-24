#pragma once
class Physics
{
private:
	float velOld[3], velNew[3];
	float accelOld[3], accelNew[3];
	float posOld[3], posNew[3];
public:
	void computeNew(float, float[3], float);
	float getPos(int a) { return posNew[a]; }
	Physics();
	~Physics();
};

