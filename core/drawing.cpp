#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL_opengl.h>

#include "../includes/drawing.h"
#include "SOIL/SOIL.h"



texture2D::texture2D() {
    glGenTextures(1, &this->id);
}

texture2D::texture2D(const GLchar *path) {
    glGenTextures(1, &this->id);
    load(path);
}

texture2D::~texture2D() {
    glDeleteTextures(1, &this->id);
}

int texture2D::load(const GLchar *path) const {
    int w,h;
    unsigned char*  image = SOIL_load_image(path,&w,&h,0,SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, this->id); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    return 0;
}

GLuint texture2D::getId() const {
    return id;
}

GLint texture2D::getUniform() const {
    return uniform;
}

void texture2D::setUniform(GLint uniform) {
    texture2D::uniform = uniform;
}


