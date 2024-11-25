#version 330 core

in vec2 TexCoords; 
uniform sampler2D texture1; 
out vec4 FragColor; 

void main() {
    vec4 texColor = texture(texture1, TexCoords);

    if ((texColor.g > 0.41 && texColor.r < 0.60 && texColor.b < 0.32)
        && TexCoords.y < 0.75) { 
        texColor.a = 0.5;
    }

    FragColor = texColor;
}