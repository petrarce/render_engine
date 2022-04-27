#include <RigidBodySystem.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;
void RigidBodySystem::updatePositions(const float timeStep)
{
	for(int i = 0; i < rb_list.size(); i++){
		rb_list[i].update(timeStep);
		rb_list[i].setColor(1);
	}
}

void RigidBodySystem::updateVelocities(const float timeStep) 
{

} 

void RigidBodySystem::detectColisions(){
	collision_pairs.resize(0);
	collision_pairs.reserve(rb_list.size()*rb_list.size()/2);
	for(int i = 0; i < rb_list.size()-1; i++){
		for(int j = i + 1; j < rb_list.size(); j++){
			CollisionNormal normal;
			CollisionPoint point;
			if(rb_list[i].isColliding(rb_list[j], normal, point)) {
				collision_pairs.push_back(make_pair(
											RigidBodyIndeces({i,j}), 
											make_pair(normal, point)
											));
				rb_list[i].setColor(0.5);
				rb_list[j].setColor(0.5);
			}
		}
	}
}

void RigidBodySystem::update(const float timeStep) 
{
	updateVelocities(timeStep);
	updatePositions(timeStep);
	detectColisions();
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
