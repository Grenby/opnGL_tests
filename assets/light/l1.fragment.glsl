#version 330 core

struct material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct light{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform material u_mat;
uniform light u_light;

varying vec3 v_normal;
varying vec3 v_worldPos;


void main(){
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_light.pos - v_worldPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(-v_worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_mat.shininess);

    vec3 ambient =  u_light.ambient * u_mat.ambient;
    vec3 diffuse = u_light.diffuse * (diff * u_mat.diffuse);
    vec3 specular = u_light.specular * (spec * u_mat.specular);

    gl_FragColor = vec4(ambient + diffuse + specular, 1);
}