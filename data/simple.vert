#version 330 core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

layout(location = 0) in vec3 vVertex;	//object space vertex position
layout(location = 1) in vec4 vColor;	//per-vertex colour
layout(location = 2) in vec4 vNormal;	//per-vertex normals
layout(location = 3) in vec2 vTexcoord;	//per-vertex texcoord

#ifdef VULKAN
layout(set = 0, binding = 0) uniform UniformBlock
{
  mat4 MVP;	// combined modelview projection matrix
  vec4 colour;
} uniformBuffer;

layout(location = 0) out vec4 vSmoothColor;		//smooth colour to fragment shader
layout(location = 1) out vec2 vSmoothTexcoord;
#else
//output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader
smooth out vec2 vSmoothTexcoord;

//uniform
uniform mat4 MVP;	//combined modelview projection matrix
uniform vec4 colour;
#endif

void main()
{
#ifdef VULKAN
    vec4 colour = uniformBuffer.colour;
    mat4 MVP = uniformBuffer.MVP;
#endif
    // assign the per-vertex colour to vSmoothColor varying
    //vSmoothColor = vec4(vColor) * colour;
    vSmoothColor = vec4(vColor);
    //vSmoothColor = colour;
    vSmoothTexcoord = vTexcoord;

    //get the clip space position by multiplying the combined MVP matrix with the object space
    //vertex position
    gl_Position = MVP * vec4(vVertex, 1.0);
}

