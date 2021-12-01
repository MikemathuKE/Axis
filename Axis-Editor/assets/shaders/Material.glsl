#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
            
out vec2 v_TexCoord;
            
void main() 
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core
layout(location = 0) out vec4 color;

struct Material {
    sampler2D Diffuse;
    sampler2D Specular;
    float Shininess;
};
uniform Material material;

in vec2 v_TexCoord;
            
void main() 
{
    // Ambient
    //vec3 ambient = vec3(texture(material.Diffuse, v_TexCoord));
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f);

    // Diffuse
    vec3 diffuse = vec3(texture(material.Diffuse, v_TexCoord));

    //Specular
    vec3 specular = vec3(texture(material.Specular, v_TexCoord));

    color = vec4(ambient + diffuse + specular, 1.0f);
}