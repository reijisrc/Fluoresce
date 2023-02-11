<type>[vertex]
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_Position.xyz, 1.0);
	v_TexCoord = a_TexCoord;
}

<type>[fragment]
#version 450 core

layout(location = 0) in vec2 v_TexCoord;

layout(std140, binding = 1) uniform Environment
{
	float u_Gamma;
	float u_Exposure;
};

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
	vec3 hdrColor = texture(u_Texture, v_TexCoord).rgb;
	vec3 mapped = vec3(1.0) - exp(-hdrColor * u_Exposure);
    mapped = pow(mapped, vec3(1.0 / u_Gamma));

    o_Color = vec4(mapped, 1.0); 
}