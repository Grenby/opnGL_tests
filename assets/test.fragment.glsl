#version 330 core


uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

varying vec4 v_color;
varying vec2 v_texCoord;


void main() {
    vec2 texPos = vec2(v_texCoord.x, 1 - v_texCoord.y);
    gl_FragColor = mix(texture(u_texture1,texPos), texture(u_texture2,texPos),0.2);
    gl_FragColor.a = 1;
    gl_FragColor*=v_color;
}
