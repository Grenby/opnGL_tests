#version 330

attribute vec3 a_position;
attribute vec3 a_color;
attribute vec2 a_texCoord;


varying vec4 v_color;
varying vec2 v_texCoord;

void main() {
    gl_Position = vec4(a_position, 1.0);
    v_color = vec4(a_color,1);
    v_texCoord = a_texCoord;
}