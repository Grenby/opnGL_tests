#define GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "SOIL/SOIL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cstring>


#include <iostream>
#include <fstream>

#include "includes/shader.h"
#include "includes/drawing.h"

#define PI 3.1415926535f

const int WIDTH = 800;
const int HEIGHT = 600;
const glm::mat4 IDENTITY(1);

void sdl_init() {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

}

std::ostream &operator<<(std::ostream &os, const glm::mat4 &m) {
    for (int i = 0; i < 4 ; ++i) {
        os<<'(';
        for (int j=0;j<4;j++){
            os<<m[j][i]<<'|';
        }
        os<<")\n";
    }
    return os;
}
std::ostream &operator<<(std::ostream &os, const glm::mat3 &m) {
    for (int i = 0; i < 3 ; ++i) {
        os<<'(';
        for (int j=0;j<3;j++){
            os<<m[j][i]<<'|';
        }
        os<<")\n";
    }
    return os;
}
std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
    os<<'('<<v.x<<','<<v.y<<','<<v.z<<")\n";
    return os;
}
std::ostream &operator<<(std::ostream &os, const glm::vec4 &v) {
    os<<'('<<v.x<<','<<v.y<<','<<v.z<<','<<v.w<<")\n";
    return os;
}

struct l1Uniform{
    GLint u_model = -1;
    GLint u_view = -1;
    GLint u_projection = -1;
    GLint u_objectColor = -1;
    GLint u_lightColor = -1;
    GLint u_norTransform = -1;
    GLint u_lightPos = -1;

    GLint u_mat = -1;
    GLint u_light = -1;


    l1Uniform() = default;

};

struct l2Uniform{
    GLint u_model=-1;
    GLint u_view=-1;
    GLint u_projection=-1;
    GLint u_lightColor=-1;

    l2Uniform() =default;

    l2Uniform(GLint uModel, GLint uView, GLint uProjection, GLint uLightColor) : u_model(uModel), u_view(uView),
                                                                                 u_projection(uProjection),
                                                                                 u_lightColor(uLightColor) {}

};

struct material{
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    float shininess{};

    material() {}

    material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess) : ambient(
            ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

};

struct light{
    glm::vec3 pos;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    light() {}

    light(const glm::vec3 &pos, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) : pos(
            pos), ambient(ambient), diffuse(diffuse), specular(specular) {}

};

material bronze(){

    return material(
            glm::vec3(0.2125,0.1275,0.054),
            glm::vec3(0.714,0.4284,0.18144),
            glm::vec3(0.393548,0.271906,0.166721),
            0.2
    );
}

material redPlastic(){
    return material(
            glm::vec3(0,0,0),
            glm::vec3(0.5,0,0),
            glm::vec3(0.7,0.6,0.6),
            0.25
    );
}

void setMaterial(const material &m,const GLint loc){
    glUniform3f(loc, m.ambient.x,m.ambient.y,m.ambient.z);
    glUniform3f(loc+1, m.diffuse.x,m.diffuse.y,m.diffuse.z);
    glUniform3f(loc+2, m.specular.x,m.specular.y,m.specular.z);
    glUniform1f(loc+3,m.shininess);
}

void setLight(const light &l,const GLint loc){
    glUniform3f(loc,l.pos.x,l.pos.y,l.pos.z);
    glUniform3f(loc+1, l.ambient.x,l.ambient.y,l.ambient.z);
    glUniform3f(loc+2, l.diffuse.x,l.diffuse.y,l.diffuse.z);
    glUniform3f(loc+3, l.specular.x,l.specular.y,l.specular.z);
}

int main() {

    GLuint
            attrib_position = 0,
            attrib_normal = 1;

    sdl_init();

    uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    SDL_Window *window = SDL_CreateWindow("start window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                          windowFlags);
    SDL_GLContext context = SDL_GL_CreateContext(window);
//    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, WIDTH, HEIGHT);


    shader cubeSh("../assets/light/l1.vertex.glsl", "../assets/light/l1.fragment.glsl");
    glBindAttribLocation(cubeSh.getIdProgram(), attrib_position, "a_position");
    glBindAttribLocation(cubeSh.getIdProgram(), attrib_normal, "a_normal");
    cubeSh.link();

    shader lightSh("../assets/light/l2.vertex.glsl", "../assets/light/l2.fragment.glsl");
    glBindAttribLocation(cubeSh.getIdProgram(), attrib_position, "a_position");
    lightSh.link();

    GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    GLuint
            cubeVAO,lightVAO,
            idVertexBuffer;

    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &lightVAO);

    glGenBuffers(1, &idVertexBuffer);

    glBindVertexArray(cubeVAO);

    //set vertex array
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //attribute of position
    glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(0));
    glEnableVertexAttribArray(attrib_position);

    //attribute of normals
    glVertexAttribPointer(attrib_normal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(3*sizeof (GLfloat)));
    glEnableVertexAttribArray(attrib_normal);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(lightVAO);
    //set vertex array
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //attribute of position
    glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(0));
    glEnableVertexAttribArray(attrib_position);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    int run = 1;

    glm::vec3 objColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor = glm::vec3(1,1,1);
    glm::vec3 lightPos(-4,0,1);


    glm::mat4 projection = glm::perspective(45.f,(GLfloat)(WIDTH)/(GLfloat)(HEIGHT),0.1f,100.f);
    glm::mat4 view = glm::lookAt(glm::vec3(-2,0,2),glm::vec3(0,0,0),glm::vec3(0,1,0));
    glm::mat4 modelCube = glm::identity<glm::mat4>();
    glm::mat4 modelLight = glm::identity<glm::mat4>();
    modelLight = glm::translate(modelLight,lightPos);
    //modelLight = glm::rotate(modelLight,PI/4.f,glm::vec3(1,0,0));

    glm::mat3 norTransform;
    norTransform = glm::mat3(glm::transpose(glm::inverse(view * modelCube)));

    l1Uniform l1;
    l1.u_model =  glGetUniformLocation(cubeSh.getIdProgram(),"u_model");
    l1.u_view = glGetUniformLocation(cubeSh.getIdProgram(),"u_view");
    l1.u_projection = glGetUniformLocation(cubeSh.getIdProgram(), "u_projection");
    l1.u_norTransform = glGetUniformLocation(cubeSh.getIdProgram(),"u_norTransform");

    l1.u_mat  = glGetUniformLocation(cubeSh.getIdProgram(),"u_mat.ambient");
    l1.u_light =glGetUniformLocation(cubeSh.getIdProgram(),"u_light.pos");


    l2Uniform l2;
    l2.u_model =  glGetUniformLocation(lightSh.getIdProgram(),"u_model");
    l2.u_view = glGetUniformLocation(lightSh.getIdProgram(),"u_view");
    l2.u_projection = glGetUniformLocation(lightSh.getIdProgram(), "u_projection");
    l2.u_lightColor = glGetUniformLocation(lightSh.getIdProgram(),"u_lightColor");

    lightPos = (view * glm::vec4(lightPos,1));

    std::cout<<lightPos;
    light l{
        glm::vec3(lightPos),
        glm::vec3(1,1,1),
        glm::vec3(1,1,1),
        glm::vec3(1,1,1)
    };
    material m = redPlastic();
    while (run){
        SDL_Event Event;
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_KEYDOWN) {
                switch (Event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        run = 0;
                        break;
                }
            } else if (Event.type == SDL_QUIT)
                run = 0;
        }

        float angle = (float)(SDL_GetTicks()/1000.f);
        lightColor.x = (sin(angle * 2.0f)+1)/2;
        lightColor.y =(1+ sin(angle * 0.7f))/2;
        lightColor.z = (1+sin(angle * 1.3f))/2;

        l.diffuse = 0.5f* lightColor;
        l.ambient = 0.2f* l.diffuse;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeSh.bind();

        glUniformMatrix4fv(l1.u_model,1,GL_FALSE,glm::value_ptr(modelCube));
        glUniformMatrix4fv(l1.u_view,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(l1.u_projection, 1, GL_FALSE, glm::value_ptr(projection));

        glUniformMatrix3fv(l1.u_norTransform, 1, GL_FALSE, glm::value_ptr(norTransform));

        setMaterial(m,l1.u_mat);
        setLight(l,l1.u_light);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        cubeSh.unbind();

        lightSh.bind();

        glUniformMatrix4fv(l2.u_model,1,GL_FALSE,glm::value_ptr(modelLight));
        glUniformMatrix4fv(l2.u_view,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(l2.u_projection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(l2.u_lightColor, lightColor.x,lightColor.y,lightColor.z);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);

        lightSh.unbind();

        SDL_GL_SwapWindow(window);

    }


    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

