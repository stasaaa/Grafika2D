#version 330 core

in vec2 TexCoords; 
out vec4 color; 

uniform sampler2D texture1; 
uniform float whiteProgress;

void main() {
    vec4 texColor = texture(texture1, TexCoords);

    if ((texColor.g > 0.41 && texColor.r < 0.60 && texColor.b < 0.32)
        && TexCoords.y < 0.75) { 
        texColor.a = 0.5;
    }

    if (TexCoords.y >= 1.0f - whiteProgress) {
        color = mix(texColor, vec4(1.0f, 1.0f, 1.0f, texColor.a), 0.6f);
        return;
    }

    color = texColor;
}