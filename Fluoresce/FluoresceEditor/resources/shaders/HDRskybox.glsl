<type>[vertex]
#version 450 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(location = 0) out vec3 v_Position;

void main()
{
	vec4 position = vec4(a_Position.xy, 1.0, 1.0);
	gl_Position = position;
	
	mat4 inverseVP = inverse(u_ViewProjection);
	v_Position = (inverseVP * position).xyz;
	
}

<type>[fragment]
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout(location = 0) in vec3 v_Position;

layout(binding = 0) uniform samplerCube u_EnvironmentTexture;

void main()
{
	vec3 envColor = texture(u_EnvironmentTexture, v_Position).rgb;
	o_Color = vec4(envColor, 1.0);
	o_EntityID = -1;
}