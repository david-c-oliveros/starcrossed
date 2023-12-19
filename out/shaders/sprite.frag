#version 330 core

in vec2 vTexCoords;
out vec4 vColor;

uniform bool bTextured;
uniform sampler2D sImage;
uniform vec3 vSpriteColor;

void main()
{
    if (bTextured)
        vColor = vec4(vSpriteColor, 1.0) * texture(sImage, vTexCoords);
    else
        vColor = vec4(vSpriteColor, 1.0);
}
