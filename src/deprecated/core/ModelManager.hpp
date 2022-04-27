#pragma once
#include <Manager.hpp>
#include <Model.hpp>
#include <Mesh.hpp>
#include <MeshManager.hpp>
#include <string>

class ModelManager : public Manager<std::string, Model, ModelManager>
{
public:
    static void createItem(const std::string modelName)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(modelName) == manager.mItemList.end())
            manager.mItemList[modelName] = std::move(std::unique_ptr<Model>(new Model()));
        else
            pr_warn("%s model already exists", modelName.c_str());
    }

    static void addModelFromPath(std::string modelName, std::string pathToModel)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(modelName) != manager.mItemList.end())
            manager.mItemList[modelName]->loadModel(pathToModel);
        else
            pr_warn("%s model does not exists yet", modelName.c_str());
    }
    
    static void createItemFromFile(const std::string pathToModel)
    {
        auto& manager = getInstance();
        createItem(pathToModel);
        addModelFromPath(pathToModel, pathToModel);
    }

    static void deleteItem(std::string modelName)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(modelName) != manager.mItemList.end())
            manager.mItemList.erase(modelName);
    }

    static const std::vector<MeshID>& getMeshList(std::string modelName)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(modelName) != manager.mItemList.end())
            return manager.mItemList[modelName]->getMeshList();
        else
            pr_warn("%s model does not exists yet", modelName.c_str());
        
        assert(0 && "mesh list is empty");
    }
};
