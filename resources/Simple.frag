#version 330
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;

in vec3 VertColor;

void main(void) {
    FragColor = vec4(VertColor,1.0);
}