#version 330
out vec4 FragColor;

in vec3 VertColor;

void main(void) {
    FragColor = vec4(VertColor,1.0);
}