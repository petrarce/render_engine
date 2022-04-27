#include <Shader.hpp>

class UpdateMethod {
	virtual void update(const float timeStep) = 0;
	virtual void draw(Shader& shader) = 0;
};