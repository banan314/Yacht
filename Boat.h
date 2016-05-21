#pragma once
class Boat
{
private:
	float posX, posY, posZ;
	float mass;
protected:
	void kadlub();
	void dziob();
	void maszt(float, float);
	void rufa();
	void zagiel(float, float);
public:
	Boat();
	float getMass() { return mass; }
	void setPosition(float, float);
	void setPosition(float, float, float);
	void renderAll();
	void renderAll(float scale);
	void renderMirror(float scale);
	void renderMirror();
	~Boat();
};

