#version 330 core

in vec2 TexCoords;
out vec4 color; 

uniform sampler2D image;  
uniform vec3 spriteColor; 
uniform vec2 textOffSet;
uniform vec2 textRepeat; 
uniform int isCircle; 
uniform int useTexture;  
uniform float alpha;

void main()
{
    vec2 modifiedTexCoords = vec2(
        (TexCoords.x + textOffSet.x) * textRepeat.x, 
        (TexCoords.y + textOffSet.y) * textRepeat.y  
    );

    if (isCircle == -1)
    {
        if (useTexture == 1)
        {
            color = vec4(spriteColor, alpha) * texture(image, modifiedTexCoords);
        }
        else
        {
            color = vec4(spriteColor, alpha);
        }
    } 
    else 
    {
        color = vec4(spriteColor, alpha);
    }
}