#pragma once
class Actor
{
public:
	Actor();
	virtual ~Actor();

	virtual void Init(){};
	virtual void Update(float dt){};
	virtual void Render(){};
};

