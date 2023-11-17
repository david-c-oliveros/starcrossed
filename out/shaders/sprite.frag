#version 330 core

in vec2 vTexCoords;
out vec4 vColor;

uniform sampler2D sImage;
uniform vec3 vSpriteColor;

void main()
{
    vColor = vec4(vSpriteColor, 1.0) * texture(sImage, vTexCoords);
}
