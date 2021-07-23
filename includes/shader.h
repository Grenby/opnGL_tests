#ifndef OPENGLTEST_SHADER_H
#define OPENGLTEST_SHADER_H

#include "SDL2/SDL_opengl.h"

class shader{
private:
    GLuint idProgram;
    bool start = false;
public:

    shader();

    shader(const GLchar * vertexPath, const GLchar * fragmentPath);

    virtual ~shader();

    int load(const GLchar * vertexPath, const GLchar * fragmentPath) const;

    int create(const GLchar * vertex, const GLchar * fragment,GLint lenV,GLint lenF) const;

    void link() const;

    void bind();

    void unbind();

    GLuint getIdProgram() const;

};



#endif //OPENGLTEST_SHADER_H
