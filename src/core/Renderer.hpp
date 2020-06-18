#pragma once
#include <Buffer.hpp>
#include <DrowableObjManager.hpp>
#include <MeshManager.hpp>
#include <ModelManager.hpp>
#include <Camera.hpp>
#include <Shader.hpp>
#include <Eigen/Dense>

class Renderer
{
public:
    static void draw(const Buffer* buffer, GLenum primitiveType)
    {
        if(!buffer)
            return;

        buffer->bind();
        unsigned int bufferSize = buffer->size();
        switch(buffer->type())
        {
        case Buffer::ArrayBufferType:
            glDrawArrays(primitiveType, 0, bufferSize); 
            break;
        case Buffer::ElementArrayBufferType:
            glDrawElements(primitiveType, bufferSize, GL_UNSIGNED_INT, 0); 
            break;
        default:
            pr_warn("unknown buffer type...");
            assert(0);
        }
        Buffer::bindDefault();
    }
    static void renderPass(const Camera& camera, Shader& lightingShader, GLenum primitiveType)
    {
        vector<DrowableID> objIds;
        DrowableObjectManager::getObjIds(objIds);
        lightingShader["view"] = camera.getView();
        lightingShader["projection"] = camera.getProjection();
        for(const DrowableID objId : objIds)
        {
            const DrowableObject& obj = DrowableObjectManager::getItem(objId);
            obj.setup(lightingShader);
            const vector<MeshID>& meshes = ModelManager::getMeshList(obj.modelName());
            for(MeshID meshID : meshes)
            {
                const Mesh& mesh = MeshManager::getItem(meshID);
                mesh.setup(lightingShader);
                draw(mesh.buffer(), primitiveType);
            }
        }
    }

    Renderer() = delete;
    ~Renderer() = delete;
};


