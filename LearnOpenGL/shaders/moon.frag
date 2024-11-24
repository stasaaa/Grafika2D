#version 330 core

in vec2 TexCoords;  
out vec4 color;    

uniform sampler2D image; 
uniform float redProgress; 
uniform int hasTexture;    
uniform int direction;   

void main() {
    vec4 texColor = hasTexture == 1 ? texture(image, TexCoords) : vec4(1.0f, 0.0f, 0.0f, 0.0f);

    if (direction == 1) {
        if (TexCoords.x <= redProgress) {
            texColor.a = hasTexture == 1 ? texColor.a : 0.2f;
            color = vec4(texColor.rgb + vec3(0.5f, 0.0f, 0.0f), texColor.a);
        } else {
            color = texColor;
        }
    } else {
        if (TexCoords.x >= 1.0f - redProgress) {
            texColor.a = hasTexture == 1 ? texColor.a : 0.2f;
            color = vec4(texColor.rgb + vec3(0.5f, 0.0f, 0.0f), texColor.a);
        } else {
            color = texColor;
        }
    }
}