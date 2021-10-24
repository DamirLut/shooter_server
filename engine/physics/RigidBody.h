//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef ENGINE_RIGIDBODY_H
#define ENGINE_RIGIDBODY_H

#include <vector>
#include <memory>
#include <functional>
#include "../Mesh.h"

class RigidBody : public Mesh {
public:
    RigidBody() = default;

    ~RigidBody() override = default;
};


#endif //INC_3DZAVR_RIGIDBODY_H
