#pragma once
#include <string>
#include <vector>
#include <Manager.hpp>
#include <Mesh.hpp>
#include <types.hpp>
#include <Shader.hpp>
typedef void* MeshID ;
class MeshManager : Manager<MeshID, 
                            Mesh,
                            MeshManager>
{
public:
    static MeshID createItem(const vector<Vertex_s>& vertices,
                           const vector<unsigned int> indeces,
                           const vector<pair<TexturePath, MapType>> textures)
    {
        unique_ptr<Mesh> newMesh(new Mesh(vertices, indeces, textures));
        if(!newMesh){
            pr_warn("failed to create new mesh");
            return 0;
        }
        auto& manager = getInstance();
        MeshID meshId = newMesh.get();
        manager.mItemList[meshId] = move(newMesh);
        return meshId;
    }
    
    static void deleteItem(MeshID key)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(key) != manager.mItemList.end())
            manager.mItemList.erase(key);
    }

    static const Mesh& getItem(MeshID key)
    {
        auto& manager = getInstance();
        if(manager.mItemList.find(key) != manager.mItemList.end())
            return *manager.mItemList[key];
        assert(0);
    }
    
};
