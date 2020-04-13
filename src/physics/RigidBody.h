#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <UpdateMethod.h>
#include <Model.hpp>
#include <glm/gtc/type_ptr.hpp>

class RigidBody : public UpdateMethod{

private:	
	float m_mass;
	Eigen::Vector3f m_inertia;
	
	Eigen::Quaternionf m_rotation;
	Eigen::Vector3f m_translation;

	Eigen::Vector3f m_velocity;
	Eigen::Vector3f m_angVelocity;

	Model m_mod;



public:
  	float mass() const ;
	const Eigen::Vector3f& inertia() const;
	
	const Eigen::Quaternionf& rotation() const;
	const Eigen::Matrix3f rotationMatr() const;
	const Eigen::Vector3f& translation() const;

	const Eigen::Vector3f& velocity() const;
	const Eigen::Vector3f& angVelocity() const;



	void setMass(const float& Mass);
	void setInertia(const Eigen::Vector3f& Inertia);
	
	void setRotation(const Eigen::Quaternionf& Rotation);
	void setTranslation(const Eigen::Vector3f& Translation);

	void setVelocity(const Eigen::Vector3f& Velocity);
	void setAngVelocity(const Eigen::Vector3f& AngVelocity);

	virtual void update(const float timeStep) override;

	virtual void draw(Shader& shader) override;

	RigidBody(	float mass, 
				const Eigen::Vector3f& rotation, 
				const Eigen::Vector3f& position,
				const Eigen::Vector3f& angVelocity,
				const Eigen::Vector3f& velocity,
				const string& path);
	~RigidBody(){};

private:
	RigidBody(){};
};