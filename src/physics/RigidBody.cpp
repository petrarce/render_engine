#include <RigidBody.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Eigen;

float RigidBody::mass() const {return m_mass;}
const Vector3f& RigidBody::inertia() const {return m_inertia;}

const Quaternionf& RigidBody::rotation() const {return m_rotation;}
const Matrix3f RigidBody::rotationMatr() const {return m_rotation.toRotationMatrix();}
const Vector3f& RigidBody::translation() const {return m_translation;}

const Vector3f& RigidBody::velocity() const {return m_velocity;}
const Vector3f& RigidBody::angVelocity() const {return m_angVelocity;}

void RigidBody::setMass(const float& Mass){m_mass = Mass;};
void RigidBody::setInertia(const Vector3f& Inertia){m_inertia = Inertia;};

void RigidBody::setRotation(const Quaternionf& Rotation){m_rotation = Rotation;};
void RigidBody::setTranslation(const Vector3f& Translation){m_translation = Translation;};

void RigidBody::setVelocity(const Vector3f& Velocity){m_velocity = Velocity;};
void RigidBody::setAngVelocity(const Vector3f& AngVelocity){m_angVelocity = AngVelocity;};

void RigidBody::update(const float timeStep) 
{
	m_translation += timeStep * m_velocity;

	Quaternionf rotQuat = Quaternionf(AngleAxisf(m_angVelocity.norm(), m_angVelocity)).normalized();
	m_rotation = rotQuat * m_rotation;
};

void RigidBody::draw(Shader& shader) 
{
	Matrix4f model;
	model << rotationMatr(), m_translation, 0, 0, 0, 1;
	auto modleMatr = glm::make_mat4(model.data());
	shader["model"] = modleMatr;
	m_mod.Draw(shader);
};

RigidBody::RigidBody(	float mass, 
			const Vector3f& rotation, 
			const Vector3f& position,
			const Vector3f& angVelocity,
			const Vector3f& velocity,
			const string& path):
	m_mass(mass),
	m_translation(position),
	m_rotation(Quaternionf(AngleAxisf(rotation.norm(), rotation)).normalized()),
	m_angVelocity(angVelocity),
	m_velocity(velocity),
	m_mod(path.c_str()),
	m_inertia(Vector3f::Identity() * mass)
{}