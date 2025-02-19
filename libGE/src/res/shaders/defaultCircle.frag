#version 330 core

in vec2 fragPos;
out vec4 FragColor;

uniform vec4 color;
uniform vec4 outlineColor;
uniform float outlineSize;

void main() {
	float dist = length(fragPos);

	if (dist > 1.0)
		discard;

	if (dist > (1.0 - outlineSize))
		FragColor = outlineColor;
	else
		FragColor = color;
}
