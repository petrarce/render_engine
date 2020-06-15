#pragma once
#include <unordered_map>
#include <memory>
#include <Singleton.hpp>
#include <cassert>
template<class Key, 
         class Item, 
         class Owner>
class Manager : public Singleton<Manager<Key, Item, Owner>>
{
    friend Owner;
protected:
    std::unordered_map<Key, std::unique_ptr<Item>> mItemList;
};
