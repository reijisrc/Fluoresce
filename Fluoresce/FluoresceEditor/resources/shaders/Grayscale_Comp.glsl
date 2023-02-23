<type>[compute]
#version 450 core

layout (local_size_x = 16, local_size_y = 9, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout (binding = 0, rgba16f) uniform image2D inputImage;
layout (binding = 1, rgba16f) uniform image2D outputImage;

void main() {
	vec4 texel;
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	texel = imageLoad(inputImage, texelCoord);
	float gray = dot(texel.rgb, vec3(0.299, 0.587, 0.114));
	vec4 color = vec4(gray, gray, gray, texel.w);
	imageStore(outputImage, texelCoord, color);
}