//
// Created by dima on 22.07.2021.
//

#ifndef OPNGL_TESTS_DRAWING_H
#define OPNGL_TESTS_DRAWING_H

#include <ostream>
#include "SDL2/SDL_opengl.h"

class texture2D{
private:
    GLuint id = 0;
    GLint uniform = -1;
public:
    texture2D();
    texture2D(const GLchar * path);
    ~texture2D();

    int load(const GLchar * path) const;

    //int create(const GLchar * vertex, const GLchar * fragment,GLint lenV,GLint lenF) const;

    GLuint getId() const;

    GLint getUniform() const;

    void setUniform(GLint uniform);

};

class cube{

};


#endif //OPNGL_TESTS_DRAWING_H
