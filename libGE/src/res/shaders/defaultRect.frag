#version 330 core

out vec4 FragColor;
uniform vec4 color;
uniform vec4 outlineColor;
uniform float outlineSize;
uniform vec2 rectSize;  // Taille réelle du rectangle en pixels

in vec2 FragPos;

void main() {
    // Convertir l'outline en espace UV
    vec2 pixelOutline = vec2(outlineSize) / rectSize;
    
    if (outlineSize > 0.0) {
        if (FragPos.x <= pixelOutline.x || FragPos.x >= 1.0 - pixelOutline.x ||
            FragPos.y <= pixelOutline.y || FragPos.y >= 1.0 - pixelOutline.y) {
            FragColor = outlineColor;
            return;
        }
    }
    
    FragColor = color;
}
