#include <RigidBodySystem.h>

void RigidBodySystem::updatePositions(const float timeStep)
{
	for(int i = 0; i < rb_list.size(); i++){
		rb_list[i].update(timeStep);
	}
}

void RigidBodySystem::updateVelocities(const float timeStep) 
{

} 

void RigidBodySystem::update(const float timeStep) 
{
	updateVelocities(timeStep);
	updatePositions(timeStep);
}

void RigidBodySystem::draw(Shader& shader)
{
	for(int i = 0; i < rb_list.size(); i++){
		rb_list[i].draw(shader);
	}
}

void RigidBodySystem::addRigidBody(const RigidBody& rb) 
{
	rb_list.push_back(rb);
}

RigidBodySystem::RigidBodySystem() :
	rb_list()
{};
RigidBodySystem::~RigidBodySystem()
{};
