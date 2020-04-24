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

	float color;

	array<float,6> m_minMaxXYZ;

	void init_obb();

public:
  	float mass() const ;
	const Eigen::Vector3f& inertia() const;
	
	const Eigen::Quaternionf& rotation() const;
	const Eigen::Matrix3f rotationMatr() const;
	const Eigen::Vector3f& translation() const;

	const Eigen::Vector3f& velocity() const;
	const Eigen::Vector3f& angVelocity() const;

	const vector<Eigen::Vector3f> obb() const;


	void setMass(const float& Mass);
	void setInertia(const Eigen::Vector3f& Inertia);

	void setRotation(const Eigen::Quaternionf& Rotation);
	void setTranslation(const Eigen::Vector3f& Translation);

	void setVelocity(const Eigen::Vector3f& Velocity);
	void setAngVelocity(const Eigen::Vector3f& AngVelocity);

	void setColor(const float val){ color = val;}

	const Eigen::Vector3f toWorldSpace(const Eigen::Vector3f& localSpacePoint) const;
	const Eigen::Vector3f toLocalSpace(const Eigen::Vector3f& worldSpacePoint) const;

	bool isColliding(const RigidBody& rb_other, 
						Eigen::Vector3f& wCollisionPoint, 
						Eigen::Vector3f& wCollisionNormal);

	
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