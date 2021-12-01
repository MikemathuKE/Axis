#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoords = a_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location=0) out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main()
{
	color = texture(u_Skybox, v_TexCoords);
}