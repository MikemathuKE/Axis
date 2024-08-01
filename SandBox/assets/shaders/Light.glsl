#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
            
out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
            
void main() 
{
    v_TexCoord = a_TexCoord;
    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core
layout(location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

in vec2 v_TexCoord;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight light;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointlight;

in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_ViewPos;
            
void main() 
{
    // Point Light
    //float distance = length(light.position - v_FragPos);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    //ambient *= attenuation;

    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    //diffuse *= attenuation;

    //Specular
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
    //specular *= attenuation;

    color = vec4(ambient + diffuse + specular, 1.0f);
}