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

const vector<Vector3f> RigidBody::obb() const
{
	vector<Vector3f> obb(8);
	obb[0] = Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[1], m_minMaxXYZ[2]);
	obb[1] = Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[1], m_minMaxXYZ[5]);
	obb[2] = Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[4], m_minMaxXYZ[2]);
	obb[3] = Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[4], m_minMaxXYZ[5]);
	obb[4] = Vector3f(m_minMaxXYZ[3], m_minMaxXYZ[1], m_minMaxXYZ[2]);
	obb[5] = Vector3f(m_minMaxXYZ[3], m_minMaxXYZ[1], m_minMaxXYZ[5]);
	obb[6] = Vector3f(m_minMaxXYZ[3], m_minMaxXYZ[4], m_minMaxXYZ[2]);
	obb[7] = Vector3f(m_minMaxXYZ[3], m_minMaxXYZ[4], m_minMaxXYZ[5]);
	return obb;

}

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
	shader["defaultDiffuse"] = glm::vec3(color, color, color);
	m_mod.Draw(shader);
};

void RigidBody::init_obb()
{

	for(const MeshID mshId  : m_mod.getMeshList()){
		const auto& msh = MeshManager::getItem(mshId);
		const vector<Vertex_s>& vertices = msh.vertices;
		if(vertices.empty()){
			continue;
		}
		float maxX, minX, maxY, minY, maxZ, minZ;
		maxX = minX = vertices[0].Position[0];
		maxY = minY = vertices[0].Position[1];
		maxZ = minZ = vertices[0].Position[2];

		for(int i = 1; i < vertices.size(); i++){
			if(vertices[i].Position[0] > maxX){
				maxX = vertices[i].Position[0];
			} else if(vertices[i].Position[0] < minX) {
				minX = vertices[i].Position[0];
			}

			if(vertices[i].Position[1] > maxY){
				maxY = vertices[i].Position[1];
			} else if(vertices[i].Position[1] < minY) {
				minY = vertices[i].Position[1];
			}

			if(vertices[i].Position[2] > maxZ){
				maxZ = vertices[i].Position[2];
			} else if(vertices[i].Position[2] < minZ) {
				minZ = vertices[i].Position[2];
			}
		}

		m_minMaxXYZ[0] = (m_minMaxXYZ[0] > maxX)?m_minMaxXYZ[0]:maxX;
		m_minMaxXYZ[1] = (m_minMaxXYZ[1] > maxY)?m_minMaxXYZ[1]:maxY;
		m_minMaxXYZ[2] = (m_minMaxXYZ[2] > maxZ)?m_minMaxXYZ[2]:maxZ;

		m_minMaxXYZ[3] = (m_minMaxXYZ[3] < minX)?m_minMaxXYZ[3]:minX;
		m_minMaxXYZ[4] = (m_minMaxXYZ[4] < minY)?m_minMaxXYZ[4]:minY;
		m_minMaxXYZ[5] = (m_minMaxXYZ[5] < minZ)?m_minMaxXYZ[5]:minZ;

	}
}

const Vector3f RigidBody::toWorldSpace(const Vector3f& localSpacePoint) const
{
	return m_rotation.toRotationMatrix() * localSpacePoint + m_translation;
}

const Vector3f RigidBody::toLocalSpace(const Vector3f& worldSpacePoint) const
{
	return m_rotation.toRotationMatrix().transpose() * (worldSpacePoint - m_translation);
}

static bool pointSetsIntersect(		vector<Vector3f>& ptSet1, 
									vector<Vector3f>&& ptSet2, 
									const Vector3f& axe, 
									const Vector3f& origin )
{
	auto compare = [&axe, &origin](const Vector3f& pt1, const Vector3f& pt2)
			{
				Vector3f v1 = pt1 - origin;
				Vector3f v2 = pt2 - origin;
				return v1.dot(axe) < v2.dot(axe);
			};
	auto squaredLength = [&axe, &origin](const Vector3f& pt1, const Vector3f& pt2)
			{
				Vector3f v1 = pt1 - origin;
				Vector3f v2 = pt2 - origin;
				return (v1.dot(axe)*axe - v2.dot(axe)*axe).norm();//.squaredNorm();
			};
	sort(ptSet1.begin(), ptSet1.end(), compare);
	sort(ptSet2.begin(), ptSet2.end(), compare);

	Vector3f minPt = (compare(ptSet1.front(), ptSet2.front()))?ptSet1.front():ptSet2.front();
	Vector3f maxPt = (compare(ptSet1.back(), ptSet2.back()))?ptSet2.back():ptSet1.back();

	float ptSet1Length 	= 	squaredLength(ptSet1.front(), ptSet1.back());
	float ptSet2Length	=	squaredLength(ptSet2.front(), ptSet2.back());
	float totalLength 	=  	squaredLength(minPt, maxPt);

	if(ptSet1Length + ptSet2Length < totalLength){
		return false;
	}
	return true;
}

bool RigidBody::isColliding(const RigidBody& rbOther, Vector3f& wCollisionPoint, Vector3f& wCollisionNormal) {
	vector<Vector3f> obbOther = rbOther.obb();
	vector<Vector3f> obbThis = obb();
	
	//convert obb coordinates to local space ot current rigid body
	for(Vector3f& obbOtherPt : obbOther) {
		obbOtherPt = toLocalSpace(rbOther.toWorldSpace(obbOtherPt));
	}

	Vector3f obbOrigin = Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[1], m_minMaxXYZ[2]); 
	Vector3f axe1 = (Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[1], m_minMaxXYZ[5]) - obbOrigin).normalized();
	Vector3f axe2 = (Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[4], m_minMaxXYZ[2]) - obbOrigin).normalized();
	Vector3f axe3 = (Vector3f(m_minMaxXYZ[3], m_minMaxXYZ[1], m_minMaxXYZ[2]) - obbOrigin).normalized();

	Vector3f obbOriginOther = obbOther[0];
	Vector3f axeOther1 = (obbOther[1] - obbOriginOther).normalized();
	Vector3f axeOther2 = (obbOther[2] - obbOriginOther).normalized();
	Vector3f axeOther3 = (obbOther[4] - obbOriginOther).normalized();

	if( pointSetsIntersect(obbOther, {obbOrigin, Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[1], m_minMaxXYZ[5])}, axe1, obbOrigin) &&
		pointSetsIntersect(obbOther, {obbOrigin, Vector3f(m_minMaxXYZ[0], m_minMaxXYZ[4], m_minMaxXYZ[2])}, axe2, obbOrigin) &&
		pointSetsIntersect(obbOther, {obbOrigin, Vector3f(m_minMaxXYZ[3], m_minMaxXYZ[1], m_minMaxXYZ[2])}, axe3, obbOrigin) &&
		pointSetsIntersect(obbThis, {obbOriginOther, obbOther[1]}, axeOther1, obbOriginOther) &&
		pointSetsIntersect(obbThis, {obbOriginOther, obbOther[2]}, axeOther2, obbOriginOther) &&
		pointSetsIntersect(obbThis, {obbOriginOther, obbOther[3]}, axeOther3, obbOriginOther)){
		return true;
	}
	return false;

}

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
{
	init_obb();
}
