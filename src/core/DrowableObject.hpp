#pragma once
#include <ModelManager.hpp>
class DrowableObject
{
private:
    std::string mModelName;
    glm::mat4 mModelMatrix;
public:
    const glm::mat4& modelMatrix() const { return mModelMatrix;}
    const std::string& modelName() const { return mModelName;}
    void setModel(const std::string& modelName)
    {
        mModelName = modelName;
    }
    void setModelMatrix(const glm::mat4& modelMatrix)
    {
        mModelMatrix = modelMatrix;
    }

    DrowableObject(){}
    
};
