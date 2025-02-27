#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

smooth in vec3 outputColour;

out vec4 frag_colour;

void main()
{
    frag_colour = vec3(0.0f, 1.0f, 0.0f);//outputColour;
}

