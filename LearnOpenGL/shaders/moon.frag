#version 330 core

in vec2 TexCoords;  // Normalized texture coordinates (0.0 to 1.0)
out vec4 color;     // Final output color

uniform sampler2D image;      // The texture sampler
uniform float redProgress;    // Progress of red effect (0.0 to 1.0)
uniform int hasTexture;       // Whether the texture is available (1 or 0)
uniform int direction;        // Direction of the red effect (1: applying, -1: removing)

void main() {
    vec4 texColor = hasTexture == 1 ? texture(image, TexCoords) : vec4(1.0f, 0.0f, 0.0f, 0.0f);

    if (direction == 1) {
        // Applying red tint from left to right
        if (TexCoords.x <= redProgress) {
            texColor.a = hasTexture == 1 ? texColor.a : 0.2f;
            color = vec4(texColor.rgb + vec3(0.5f, 0.0f, 0.0f), texColor.a);
        } else {
            color = texColor;
        }
    } else {
        // Removing red tint from right to left
        if (TexCoords.x >= 1.0f - redProgress) {
            texColor.a = hasTexture == 1 ? texColor.a : 0.2f;
            color = vec4(texColor.rgb + vec3(0.5f, 0.0f, 0.0f), texColor.a);
        } else {
            color = texColor;
        }
    }
}