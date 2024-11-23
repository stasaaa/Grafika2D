#include "game_object.h"


GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Alpha(1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float alpha, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Alpha(alpha), Color(color), Rotation(0.0f), 
    Sprite(sprite), IsSolid(false), Destroyed(false) {
}

void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Alpha ,glm::vec2(0.0f), 
        glm::vec2(1.0f),this->Rotation, this->Color);
}
bool GameObject::IsClicked(float mouseX, float mouseY) const
{
    return (mouseX >= this->Position.x &&
        mouseX <= this->Position.x + this->Size.x &&
        mouseY >= this->Position.y &&
        mouseY <= this->Position.y + this->Size.y);
}