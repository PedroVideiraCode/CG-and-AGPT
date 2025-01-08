#version 330 core

// Input UV interpolados do vertex shader
in vec2 UV;

// Sa�da de cor
out vec4 outColor; 

// Valores uniformes que permanecem constantes para toda a malha.
uniform sampler2D myTextureSampler;
uniform vec3 keyColor;  // Cor-chave para subtrair

void main() {
    // Obt�m a cor da textura
    vec4 texColor = texture(myTextureSampler, UV);

    // Calcula a dist�ncia entre a cor do pixel e a cor-chave
    float distance = length(texColor.rgb - keyColor);

    // Define a cor final
    outColor = texColor;
    outColor.a = 0.5;
}