#version 330 core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

// Thanks to Gerdelan, Anton. Anton's OpenGL 4 Tutorials (p. 364). Kindle Edition.

layout(location = 0) in vec3 vVertex;	//object space vertex position
layout(location = 1) in vec4 vColor;	//per-vertex colour
layout(location = 2) in vec4 vNormal;	//per-vertex normals
layout(location = 3) in vec2 vTexcoord;	//per-vertex texcoord
layout(location = 6) in int  vGlyph;	//glyph per vertex

#ifdef VULKAN
layout(std140, binding = 0) uniform UniformBlock
{
  vec2 pos;
  float scaleX;
  float scaleY;
  int width;
  int height;
  int totalWidth;
  int totalHeight;
  int index;
  int screenWidth;
  int screenHeight;
  int flip;
  int flipVertical;
  float time;
} uniformBuffer;

// data for fragment shader
/*out gl_PerVertex
{
  vec2 vSmoothTexcoord;
  float o_index;
  float o_width;
  float o_height;
  float o_totalwidth;
  float o_totalheight;
  float o_flip;
  float o_flipVertical;
  float o_time;
}*/

layout(location = 0) out vec2 vSmoothTexcoord;
layout(location = 1) out float o_index;
layout(location = 2) out float o_width;
layout(location = 3) out float o_height;
layout(location = 4) out float o_totalwidth;
layout(location = 5) out float o_totalheight;
layout(location = 6) out float o_flip;
layout(location = 7) out float o_flipVertical;
layout(location = 8) out float o_time;

#else
smooth out vec2 vSmoothTexcoord;

uniform vec2 pos;
uniform float scaleX;
uniform float scaleY;
uniform int width;
uniform int height;
uniform int totalWidth;
uniform int totalHeight;
uniform int index;
uniform int screenWidth;
uniform int screenHeight;
uniform int flip;
uniform int flipVertical;
uniform float time;
//uniform vec2 rotation;

out float o_index;
out float o_width;
out float o_height;
out float o_totalwidth;
out float o_totalheight;
out float o_flip;
out float o_flipVertical;
out float o_time;
//out vec2 o_rotation;
#endif

void main()
{
#ifdef VULKAN
    vec2 pos = uniformBuffer.pos;
    float scaleX = uniformBuffer.scaleX;
    float scaleY = uniformBuffer.scaleY;
    int width = uniformBuffer.width;
    int height = uniformBuffer.height;
    int totalWidth = uniformBuffer.totalWidth;
    int totalHeight = uniformBuffer.totalHeight;
    int index = uniformBuffer.index;
    int screenWidth = uniformBuffer.screenWidth;
    int screenHeight = uniformBuffer.screenHeight;
    int flip = uniformBuffer.flip;
    int flipVertical = uniformBuffer.flipVertical;
    float time = uniformBuffer.time;
#endif
    float aspectRatio = float(screenWidth) / float(screenHeight);

    float w = float(width)  * float(scaleX);
    float h = float(height) * float(scaleY);

    // not sure if aspectRatio stuff makes any sense
    gl_Position = vec4((vVertex.x / float(screenWidth)) * w, (vVertex.y / float(screenHeight)) * h, 0.0, 1.0);
    vSmoothTexcoord = vTexcoord;

    float halfScreenWidth  = float(screenWidth)  / 2.0f;
    float halfScreenHeight = float(screenHeight) / 2.0f;

    gl_Position.x += -1.0 + (float(width  * scaleX) / 2.0f) / halfScreenWidth   + pos.x / (halfScreenWidth);
    gl_Position.y -= -1.0 + (float(height * scaleY) / 2.0f) / halfScreenHeight  + pos.y / (halfScreenHeight);// - 11)); // 11 is the top of a window on windows
    //gl_Position.y += time;

    //gl_Position.x = gl_Position.x * rotation.y + gl_Position.y * rotation.x;
    //gl_Position.y = gl_Position.y * rotation.x + gl_Position.x * rotation.y;

    if (vGlyph == -1)
    {
        o_index = float(index);
    }
    else
    {
        o_index = float(vGlyph);
    }

    o_width = float(width);
    o_height = float(height);
    o_totalwidth = float(totalWidth);
    o_totalheight = float(totalHeight);
    o_flip = float(flip);
    o_flipVertical = float(flipVertical);
    o_time = float(time);
    //o_rotation = rotation;
}
