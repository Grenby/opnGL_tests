#version 330 core

attribute vec3 a_position;
attribute vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_norTransform;

varying vec3 v_normal;
varying vec3 v_worldPos;

void main(){
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
    v_normal = u_norTransform * a_normal;
    v_worldPos = (u_view * u_model * vec4(a_position, 1.0f)).xyz;
}