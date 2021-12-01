#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
            
out vec3 v_FragPos;
out vec3 v_Normal;

void main() 
{
    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location=0) out vec4 color;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_ViewPos;
uniform samplerCube u_Skybox;

void main()
{
	vec3 I = normalize(v_FragPos - u_ViewPos);
	vec3 R = reflect(I, normalize(v_Normal));
	color = vec4(texture(u_Skybox, R).rgb, 1.0);
}