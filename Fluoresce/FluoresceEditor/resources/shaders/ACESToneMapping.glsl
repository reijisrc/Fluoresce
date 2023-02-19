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

vec3 gammaCorrect(vec3 color, float gamma) 
{
    return pow(color, vec3(1.0/gamma));
}

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
mat3 ACESInputMat =
{
    {0.59719, 0.07600, 0.02840},
    {0.35458, 0.90834, 0.13383},
    {0.04823, 0.01566, 0.83777}
};

// ODT_SAT => XYZ => D60_2_D65 => sRGB
mat3 ACESOutputMat =
{
    { 1.60475, -0.10208, -0.00327},
    {-0.53108,  1.10813, -0.07276},
    {-0.07367, -0.00605,  1.07602 }
};

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

void main()
{
	vec3 hdrColor = u_Exposure * texture(u_Texture, v_TexCoord).rgb;
	    
    vec3 color = ACESInputMat * hdrColor;
         color = RRTAndODTFit(color);
         color = ACESOutputMat * color;

         color = gammaCorrect(color, u_Gamma);
         color = clamp(color, 0.0, 1.0);

	o_Color = vec4(color, 1.0);
}