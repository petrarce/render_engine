#include "GLMaterial.hpp"

namespace dream
{
namespace renderer
{

const Eigen::Vector4f GLMeshWithMaterialRenderFunction::mDefaultColor = {
	1.0f, 0.f, 0.f, 1.f
};

const glwrapper::GLVertexArray::AttributeSpecification
	GLMeshWithMaterialRenderFunction::verticesAttributeSpec =
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 3,
			.instanceLevel = 0,
			.location	   = 0,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 3,
			.type		   = GL_FLOAT,
		};

const glwrapper::GLVertexArray::AttributeSpecification
	GLMeshWithMaterialRenderFunction::textureAttributeSpec =
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 2,
			.instanceLevel = 0,
			.location	   = 1,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 2,
			.type		   = GL_FLOAT,
		};

const glwrapper::GLVertexArray::AttributeSpecification
	GLMeshWithMaterialRenderFunction::normalsAttributeSpec =
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 3,
			.instanceLevel = 0,
			.location	   = 2,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 3,
			.type		   = GL_FLOAT,
		};
const glwrapper::GLVertexArray::AttributeSpecification
	GLMeshWithMaterialRenderFunction::tangentAttributSpec =
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 3,
			.instanceLevel = 0,
			.location	   = 3,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 3,
			.type		   = GL_FLOAT,
		};
const glwrapper::GLVertexArray::AttributeSpecification
	GLMeshWithMaterialRenderFunction::bitangentAttributSpec =
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 3,
			.instanceLevel = 0,
			.location	   = 4,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 3,
			.type		   = GL_FLOAT,
		};

const glwrapper::GLVertexArray::AttributeSpecification
	GLMeshWithMaterialRenderFunction::vertexColorAttributeSpec =
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 4,
			.instanceLevel = 0,
			.location	   = 5,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 4,
			.type		   = GL_FLOAT,
		};

} // namespace renderer
} // namespace dream
