#pragma once
#include <RigidBody.h>

class RigidBodySystem : UpdateMethod {
private:
	vector<RigidBody> rb_list;
	
	void updatePositions(const float timeStep);

	void updateVelocities(const float timeStep);
public:
	virtual void update(const float timeStep) override;

	void draw(Shader& shader) override;

	void addRigidBody(const RigidBody& rb);

	RigidBodySystem();
	~RigidBodySystem();
};