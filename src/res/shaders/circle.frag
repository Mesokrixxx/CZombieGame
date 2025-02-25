#version 330 core

out vec4 FragColor;

uniform vec4 color;
uniform vec4 outlineColor;
uniform float outlineSize;

void main() {
	FragColor = color;
}
