#pragma once
#include <DrowableObject.hpp>
#include <Manager.hpp>

typedef void* DrowableID;
class DrowableObjectManager : public Manager<DrowableID, DrowableObject, DrowableObjectManager>
{
public:
    static DrowableID createItem()
    {
        auto& manager = getInstance();
        std::unique_ptr<DrowableObject> newObject(new DrowableObject);
        assert(newObject != nullptr);
        DrowableID id = newObject.get();
        manager.mItemList[id] = move(newObject);
        return id;
    }
    static void deleteItem(DrowableID id)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(id) != manager.mItemList.end())
            manager.mItemList.erase(id);
    }
    static void setModelMatrix(DrowableID id, const glm::mat4& matrix)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(id) != manager.mItemList.end())
            manager.mItemList[id]->setModelMatrix(matrix);            
    }
    
    static void setModel(DrowableID id, std::string modelName)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(id) != manager.mItemList.end())
            manager.mItemList[id]->setModel(modelName);            
    }
    
    static const DrowableObject& getItem(DrowableID objId)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(objId) != manager.mItemList.end())
            return *manager.mItemList[objId];
        assert(0);
    }
    
    static const void getObjIds(vector<DrowableID>& idList)
    {
        auto& manager = getInstance();
        for(const auto& item : manager.mItemList)
            idList.push_back(item.first);
    }

};
