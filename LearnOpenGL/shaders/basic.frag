#version 330 core

in vec2 TexCoords;          // Texture coordinates passed from the vertex shader
out vec4 color;             // Output color of the fragment

uniform sampler2D image;    // The texture sampler
uniform vec3 spriteColor;   // The color to multiply with the texture
uniform vec2 textOffSet;    // Offset for texture (x and y)
uniform vec2 textRepeat;    // Repeat factor for texture (x and y)
uniform int isCircle;       // Control whether to render as a circle or not
uniform int useTexture;     // Indicates whether a texture should be used (1 for yes, 0 for no)
uniform float alpha;

void main()
{
    vec2 modifiedTexCoords = vec2(
        (TexCoords.x + textOffSet.x) * textRepeat.x,  // Apply offset and repeat for x
        (TexCoords.y + textOffSet.y) * textRepeat.y   // Apply offset and repeat for y
    );

    if (isCircle == -1) // Render with texture
    {
        if (useTexture == 1)
        {
            // Use the texture color
            color = vec4(spriteColor, alpha) * texture(image, modifiedTexCoords);
        }
        else
        {
            // Render with sprite color only
            color = vec4(spriteColor, alpha);
        }
    } 
    else // Render as a solid color for the circle
    {
        color = vec4(spriteColor, alpha);
    }
}