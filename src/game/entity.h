#pragma once

#include <cstdint>

class Entity {
public:
    Entity(uint32_t id) : id_(id) {}
    
    uint32_t GetID() const { return id_; }
    
    bool operator==(const Entity& other) const {
        return id_ == other.id_;
    }
    
    bool operator!=(const Entity& other) const {
        return id_ != other.id_;
    }
    
private:
    uint32_t id_;
};

