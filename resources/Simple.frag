#version 330

in vec3 VertColor;

out vec4 FragColor;

void main(void) {
    FragColor = vec4(VertColor,1);
}