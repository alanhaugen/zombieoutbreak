#version 450
//output variable to the fragment shader
layout (location = 0) out vec3 outColor;

layout(set = 0, binding = 0) uniform UniformBlock
{
  mat4 MVP;	// combined modelview projection matrix
  vec4 colour;
} uniformBuffer;

void main()
{
        //const array of positions for the triangle
        const vec3 positions[3] = vec3[3](
                vec3(1.f,1.f, 0.0f),
                vec3(-1.f,1.f, 0.0f),
                vec3(0.f,-1.f, 0.0f)
        );

        //output the position of each vertex
        gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
        outColor = uniformBuffer.colour.xyz;
}
