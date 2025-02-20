#version 330 core

out vec4 FragColor;
uniform vec4 color;
uniform vec4 outlineColor;
uniform float outlineSize;
uniform vec2 crossSize;

in vec2 FragPos;

void main() {
    // Calculer la distance aux bords des deux rectangles qui forment la croix
    float thickness = 0.4; // Doit correspondre à la valeur dans CreateCrossVAO
    float halfThickness = thickness / 2.0;
    
    // Distance pour la barre horizontale
    float distH = abs(FragPos.y);
    bool inHorizontal = abs(FragPos.x) <= 1.0 && distH <= halfThickness;
    
    // Distance pour la barre verticale
    float distV = abs(FragPos.x);
    bool inVertical = abs(FragPos.y) <= 1.0 && distV <= halfThickness;
    
    // Convertir l'outline en espace de coordonnées normalisé
    float pixelOutline = outlineSize / min(crossSize.x, crossSize.y);
    
    if (inHorizontal || inVertical) {
        // Si on est près du bord d'une des barres
        if (inHorizontal && (distH >= halfThickness - pixelOutline || abs(FragPos.x) >= 1.0 - pixelOutline) ||
            inVertical && (distV >= halfThickness - pixelOutline || abs(FragPos.y) >= 1.0 - pixelOutline)) {
            FragColor = outlineColor;
        } else {
            FragColor = color;
        }
    } else {
        discard;
    }
}
