//
// Created by dima on 22.07.2021.
//

#ifndef OPNGL_TESTS_CAMERA_H
#define OPNGL_TESTS_CAMERA_H

#include "SDL2/SDL_opengl.h"
#include <glm/glm.hpp>

class camera{
private:
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 combined;// = proj * view
public:

    const glm::mat4 &getView() const {
        return view;
    }

    const glm::mat4 &getProj() const {
        return proj;
    }

    const glm::mat4 &getCombined() const {
        return combined;
    }


};


#endif //OPNGL_TESTS_CAMERA_H
