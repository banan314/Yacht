#pragma once
class Physics
{
private:
	float velOld[3], velNew[3];
	float accelOld[3], accelNew[3];
	float posOld[3], posNew[3];

	float force[3];
public:
	void computeNew(float, float);
	//float getPos(int a) { return posNew[a]; }
	float *getPos() { return posNew; }
	float *getVel() { return velNew; }
	float *getAccel() { return accelNew; }
	void setPos(float set[3]) { 
		for (int i = 0; i < 3; i++)
		{
			posOld[i] = posNew[i];
			posNew[i] = set[i];
		}
	}
	void setVel(float set[3]) {
		for (int i = 0; i < 3; i++)
		{
			velOld[i] = velNew[i];
			velNew[i] = set[i];
		}
	}
	void setAccel(float set[3]) {
		for (int i = 0; i < 3; i++)
		{
			accelOld[i] = accelNew[i];
			accelNew[i] = set[i];
		}
	}
	void setForce(float set[3]) {
		for (int i = 0; i < 3; i++)
		{
			force[i] = set[i];
		}
	}
	Physics();
	~Physics();
};

