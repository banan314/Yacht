#pragma once
class Boat
{
private:
	float posX, posY, posZ;
	float mass;
	const float absoluteScalingFactor = 2.4f;
	float xminOrigin, xmaxOrigin, yminOrigin, ymaxOrigin;
	float xmin, xmax, ymin, ymax;
	float angle;
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
	void renderBlender(float);
	bool isCollision(float przyladek);
	float getAngle() { return this->angle; }
	void setAngle(float a) { this->angle = a; }
	~Boat();
};

