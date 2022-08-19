#pragma once
#include <Eigen/Dense>

namespace dream
{

namespace geometry
{

class GLMesh
{

public:
	const auto &vertices() const
	{
		return mVertices;
	}
	const auto &normals() const
	{
		return mNormals;
	}
	const auto &texCoord() const
	{
		return mTexCoord;
	}
	const auto &tangents() const
	{
		return mTangents;
	}
	const auto &bitangents() const
	{
		return mBitangents;
	}
	const auto &indices() const
	{
		return mIndices;
	}

	auto &vertices()
	{
		return mVertices;
	}
	auto &normals()
	{
		return mNormals;
	}
	auto &texCoord()
	{
		return mTexCoord;
	}
	auto &tangents()
	{
		return mTangents;
	}
	auto &bitangents()
	{
		return mBitangents;
	}
	auto &indices()
	{
		return mIndices;
	}

	bool hasVertices() const
	{
		return !mVertices.empty();
	}
	bool hasNormals() const
	{
		return !mNormals.empty();
	}
	bool hasTexCoord() const
	{
		return !mTexCoord.empty();
	}
	bool hasTangents() const
	{
		return !mTangents.empty();
	}
	bool hasBitangents() const
	{
		return !mBitangents.empty();
	}
	bool hasIndices() const
	{
		return !mIndices.empty();
	}

protected:
	std::vector<Eigen::Vector3f> mVertices;
	std::vector<Eigen::Vector3f> mNormals;
	std::vector<Eigen::Vector2f> mTexCoord;
	std::vector<Eigen::Vector3f> mTangents;
	std::vector<Eigen::Vector3f> mBitangents;
	std::vector<int> mIndices;
};

} // namespace geometry

} // namespace dream
