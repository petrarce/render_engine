#pragma once
#include <RigidBody.h>
#include <vector>

class RigidBodySystem : UpdateMethod {
private:
	typedef Eigen::Vector3f CollisionNormal;
	typedef Eigen::Vector3f CollisionPoint;
	typedef array<int, 2> RigidBodyIndeces;

	vector<RigidBody> rb_list;
	vector<pair<RigidBodyIndeces, pair<CollisionNormal, CollisionPoint>>> collision_pairs;
	
	void updatePositions(const float timeStep);

	void updateVelocities(const float timeStep);

	void detectColisions();
public:
	virtual void update(const float timeStep) override;

	void draw(Shader& shader) override;

	void addRigidBody(const RigidBody& rb);

	RigidBodySystem();
	~RigidBodySystem();
};