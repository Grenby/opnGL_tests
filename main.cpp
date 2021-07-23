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

int main() {

    GLuint
        attrib_position = 0,
        attrib_color = 1,
        attrib_texCoord = 2;
    sdl_init();

    uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    SDL_Window *window = SDL_CreateWindow("start window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                                          windowFlags);
    SDL_GLContext context = SDL_GL_CreateContext(window);
//    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, WIDTH, HEIGHT);


    shader sh("../assets/test.vertex.glsl", "../assets/test.fragment.glsl");

    glBindAttribLocation(sh.getIdProgram(), attrib_position, "a_position");
    glBindAttribLocation(sh.getIdProgram(), attrib_color, "a_color");
    glBindAttribLocation(sh.getIdProgram(), attrib_texCoord, "a_texCoord");

    sh.link();


    GLfloat vertices[] = {
            // Позиции          // Цвета             // Текстурные координаты
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
    };

    GLushort indices[] = {  // Note that we start from 0!
            0, 1, 3,  // First Triangle
            1, 2, 3   // Second Triangle
    };

    GLuint
            idVertexArray,
            idPositionBuffer,
            idIndicesBuffer;

    glGenVertexArrays(1, &idVertexArray);
    glGenBuffers(1, &idPositionBuffer);
    glGenBuffers(1,&idIndicesBuffer);

    //sh.bind();
    glBindVertexArray(idVertexArray);

    //attributes of positions and colors
    glBindBuffer(GL_ARRAY_BUFFER, idPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //attribute of position
    glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(0));
    glEnableVertexAttribArray(attrib_position);
    //attribute of color
    glVertexAttribPointer(attrib_color, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,  (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(attrib_color);
    //attribute of texture coords
    glVertexAttribPointer(attrib_texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,  (GLvoid*)(6* sizeof(GLfloat)));
    glEnableVertexAttribArray(attrib_texCoord);


    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    //SOIL_load_image();
    int run = 1;

//    glUniformMatrix4fv(projMatrixLocation,1,GL_FALSE,glm::value_ptr(proj));
//    glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(view));
//    glUniform2f(const_location, addition_const.x, addition_const.y);

   // sh.unbind();

    texture2D
        t1("../assets/container.jpg"),
        t2("../assets/awesomeface.png");

    t1.setUniform(glGetUniformLocation(sh.getIdProgram(), "u_texture1"));
    t2.setUniform(glGetUniformLocation(sh.getIdProgram(), "u_texture2"));

    std::cout<<"tex1: id="<<t1.getId()<<" location="<<t1.getUniform()<<'\n';
    std::cout<<"tex2: id="<<t2.getId()<<" location="<<t2.getUniform()<<'\n';


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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT   );

        sh.bind();
//
//        glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(view));
//        glUniform2f(const_location, x,y);
//        glUniform1f(zoomLocation,angle);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t1.getId());
        glUniform1i(t1.getUniform(), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, t2.getId());
        glUniform1i(t2.getUniform(), 1);

        glBindVertexArray(idVertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT,0);
        glBindVertexArray(0);
        sh.unbind();

        SDL_GL_SwapWindow(window);

    }


    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

