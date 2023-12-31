#version 330 core

layout (location = 0) in vec4 aPos;

out vec2 vTexCoords;

uniform bool bTextured;

uniform vec2 vTexSizeInPixels;
uniform vec2 vTexSizeInSheet;
uniform vec2 vTexOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void  main()
{
    if (bTextured)
        vTexCoords = aPos.zw * vTexSizeInSheet + vTexOffset;

    gl_Position = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
}
