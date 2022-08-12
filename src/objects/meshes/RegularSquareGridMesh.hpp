#pragma once

#include "MeshObject.hpp"
#include "MeshFactory.hpp"
#include <Eigen/Dense>
namespace dream
{
namespace geometry
{

class GLRegularSquareGridMesh : public GLBuilder<GLMesh>
{
	virtual GLMesh *construct() override
	{
		GLMesh *mesh(new GLMesh());
		mesh->vertices().reserve(mCellsY * mCellsX);
		mesh->indices().reserve(mCellsY * mCellsX);

		for (int i = 0; i < mCellsX + 1; i++)
			for (int j = 0; j < mCellsY + 1; j++)
			{
				mesh->vertices().push_back({ i * mCellsX, j * mCellsY, 0 });
				mesh->texCoord().push_back({ static_cast<float>(i) / mCellsX,
											 static_cast<float>(j) / mCellsY });
			}

		for (int i = 0; i < mCellsX * mCellsY; i++)
		{
			const int iNext = (i / mCellsX + 1) * mCellsX + i % mCellsX;

			mesh->indices().push_back(i);
			mesh->indices().push_back(i + 1);
			mesh->indices().push_back(iNext);
			mesh->indices().push_back(iNext);
			mesh->indices().push_back(i + 1);
			mesh->indices().push_back(iNext + 1);
		}

		return mesh;
	};

private:
	int mCellsX;
	int mCellsY;
	Eigen::Vector2f mCellSize;
};

} // namespace geometry
} // namespace dream
