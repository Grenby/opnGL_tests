#define GL_GLEXT_PROTOTYPES

#include "../includes/shader.h"

#include <iostream>
#include <fstream>
#include <SDL2/SDL_opengl.h>

using namespace std;

shader::shader(const char *vertexPath, const char *fragmentPath) {
    this->idProgram = glCreateProgram();
    load(vertexPath,fragmentPath);
}
shader::shader() {
    this->idProgram = glCreateProgram();
}
shader::~shader() {
    glDeleteProgram(this->idProgram);
}
int shader::load(const char *vertexPath, const char *fragmentPath) const {
    GLchar *vertex;
    GLchar *fragment;
    GLint lenV;
    GLint lenF;

    fstream file;
    file.open(vertexPath, ios::out | ios::in);

    if (file.is_open()) {
        fprintf(stdout,"Vertex shader open\n");
        file.seekg(0, ios::end);
        lenV = file.tellg();
        file.seekg(0, ios::beg);
        vertex = new char[lenV];
        file.read(vertex, lenV);
        file.close();
    } else {
        fprintf(stderr, "Vertex shader don't open\n");
        return false;
    }

    file.open(fragmentPath, ios::out | ios::in);
    if (file.is_open()) {
        fprintf(stdout,"Fragment shader open\n");
        file.seekg(0, ios::end);
        lenF = file.tellg();
        file.seekg(0, ios::beg);
        fragment = new char[lenF];
        file.read(fragment, lenF);
        file.close();
    } else {
        fprintf(stderr, "Fragment shader don't open\n");
        return false;
    }
    int res = create(vertex,fragment,lenV,lenF);

    delete[] vertex;
    delete[] fragment;

    return res;
}
int shader::create(const GLchar *vertex, const GLchar *fragment, GLint lenV, GLint lenF) const {
    GLuint idVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint idFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(idVertexShader, 1, (const GLchar **) &vertex, &lenV);
    glShaderSource(idFragmentShader, 1, (const GLchar **) &fragment, &lenF);

    glCompileShader(idVertexShader);
    glCompileShader(idFragmentShader);


    GLint status;
    GLchar infoLog[512];
    glGetShaderiv(idVertexShader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        std::string m="fragment shader " + to_string(getIdProgram()) + " compilation failed\n";
        fprintf(stderr, m.c_str());
        glGetShaderInfoLog(idVertexShader, 512, NULL, infoLog);
        fprintf(stderr,infoLog);
        return false;
    }else{
        fprintf(stdout,"Vertex shader compilation is successful\n");
        glAttachShader(idProgram, idVertexShader);
    }

    glGetShaderiv(idFragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(idFragmentShader, 512, NULL, infoLog);
        std::string m="fragment shader " + to_string(getIdProgram()) + " compilation failed\n";
        fprintf(stderr, m.c_str());
        fprintf(stderr,infoLog);

        return false;
    }else {
        fprintf(stdout, "Fragment shader compilation is successful\n");
        glAttachShader(idProgram, idFragmentShader);
    }
    return true;
}

GLuint shader::getIdProgram() const {
    return idProgram;
}
void shader::link() const {
    glLinkProgram(this->idProgram);
}

void shader::bind() {
    start = true;
    glUseProgram(this->idProgram);
}

void shader::unbind() {
    if (start){
        start = false;
        glUseProgram(0);
    }
}
