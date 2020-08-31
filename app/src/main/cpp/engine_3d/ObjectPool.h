//
// Created by Igor Lapin on 03/08/2020.
//

#ifndef GAME_OBJECT_POOL_H
#define GAME_OBJECT_POOL_H


#include <memory>
#include <functional>
#include <unordered_set>
#include <exception>

template <class T>
class ObjectPool {

    std::function<std::shared_ptr<T>()> m_createObject;
    std::unordered_set<std::shared_ptr<T>> m_pool;

public:
    ObjectPool(const std::function<std::shared_ptr<T>()>& createObject) : m_createObject(createObject) {}

    std::shared_ptr<T> obtain() {
        if (m_pool.size() == 0) {
            return m_createObject();
        } else {
            auto object = *m_pool.begin();
            m_pool.erase(object);
            return object;
        }
    }

    void recycle(std::shared_ptr<T> object) {
        if (m_pool.count(object) > 0) {
            throw std::domain_error("Trying to recycle object that is already in the pool");
        }
        m_pool.insert(object);
    }
};


#endif //GAME_OBJECT_POOL_H
