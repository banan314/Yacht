#pragma once
class Tree
{
public:
	Tree();
	~Tree();
	void renderAll();
	void setPosition(float, float);
	void setPosition(float, float, float);
	void setSize(float, float);
protected:
	void konar(float rBase, float rTop, float h);
	void liscie(float rBase, float rTop, float h);
private:
	float xPos, yPos, zPos;
	float height, width;
};

