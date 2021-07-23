#version 330 core

struct material{
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct light{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 fading;
};

struct lightPoint{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 fading;
};

struct lightDir{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 fading;
};

struct lightSpot{
    vec3 pos;
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
};

uniform material u_mat;
uniform light u_light;

varying vec2 v_diffudeCoords;
varying vec3 v_normal;
varying vec3 v_worldPos;


void main(){
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_light.pos - v_worldPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(-v_worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_mat.shininess);

    vec3 d = texture(u_mat.diffuse, v_diffudeCoords).xyz;
    vec3 s = texture(u_mat.specular,v_diffudeCoords).xyz;

    float dist = length(u_light.pos - v_worldPos);
    float k = 1.0/(u_light.fading.x + dist * u_light.fading.y + dist*dist*u_light.fading.z);

    vec3 ambient =  k*u_light.ambient * d;
    vec3 diffuse = k*u_light.diffuse * (diff * d);
    vec3 specular = k*u_light.specular * (spec * s);

    gl_FragColor = vec4(ambient+diffuse+specular, 1);
}