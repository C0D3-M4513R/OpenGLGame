#version 330
#extension GL_ARB_separate_shader_objects : enable

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

uniform mat4 mvp;

out vec3 VertColor;

void main(void) {
    gl_Position = mvp * vec4(position, 1.0);
    VertColor=color;
}