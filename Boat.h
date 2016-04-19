#pragma once
class Boat
{
private:
	float posX, posY, posZ;
protected:
	void kadlub();
	void dziob();
	void maszt(float, float);
	void rufa();
	void zagiel(float, float);
public:
	Boat();
	void setPosition(float, float);
	void setPosition(float, float, float);
	void renderAll();
	~Boat();
};

