#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(aPos.xy, 0.0, 1.0);
//    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
//    gl_Position = model * vec4(aPos.xy, 0.0, 1.0);
//    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
