#include <iostream>

#include "game.h"
#include "../graphics/sprite_renderer.h"
#include "resource_manager.h"
#include <random>

SpriteRenderer* Renderer;
SpriteRenderer* CircleRenderer;
SpriteRenderer* MoonRenderer;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(60.0f, 100.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(50.0f);

const glm::vec2 SUN_SIZE(38.0f, 50.0f);
const float PULSE(2.0f);

bool SunMoving = false;
float SunAngle = 0.0f;
float SunSpeed = 50.0f;
glm::vec2 RotationCenter;
float RotationRadius = 200.0f;
glm::vec3 SkyColorDay = glm::vec3(0.5f, 0.61f, 0.97f); // Light blue
glm::vec3 SkyColorSunset = glm::vec3(1.0f, 0.5f, 0.2f); // Orange
glm::vec3 SkyColorNight = glm::vec3(0.05f, 0.05f, 0.2f); // Dark blue
glm::vec3 SkyColorMorning = glm::vec3(1.0f, 0.8f, 0.6f); // Pink
glm::vec3 SkyColor = SkyColorDay;

bool isNight = false;
bool MoonClicked = false;
bool RemoveRed = false;
float redProgress = 0.0f;
float SceneTint = 0.0f;   // 0.0f = no tint, 1.0f = full red tint

glm::vec3 CloudColor = glm::vec3(1.0f);
float CloudAlpha = 1.0f;

const float MIN_TIME_DIFF(7.0f);
const float CLOUD_VELOCITY(12.0f);

GameObject* Player;
GameObject* Sun;
GameObject* Moon;
GameObject* Tint;

int getRandomInt(int min, int max);

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/basic.vert", "shaders/basic.frag", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/moon.vert", "shaders/moon.frag", nullptr, "moon");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    CircleRenderer = new SpriteRenderer(myShader, true);

    ResourceManager::GetShader("moon").Use().SetInteger("image", 0);
    ResourceManager::GetShader("moon").SetMatrix4("projection", projection);
    ResourceManager::GetShader("moon").Use().SetFloat("redProgress", redProgress);
    ResourceManager::GetShader("moon").Use().SetInteger("direction", 1);
    Shader moonShader = ResourceManager::GetShader("moon");
    MoonRenderer = new SpriteRenderer(moonShader);

    // load textures
    ResourceManager::LoadTexture("textures/backgrounds/valley/sky_lightened.png", true, "sky_lightened");
    ResourceManager::LoadTexture("textures/backgrounds/valley/clouds_bg.png", true, "clouds_bg");
    ResourceManager::LoadTexture("textures/backgrounds/valley/glacial_mountains_lightened.png", true, "mountains");
    ResourceManager::LoadTexture("textures/backgrounds/valley/clouds_mg_3.png", true, "coulds_mg3");
    ResourceManager::LoadTexture("textures/backgrounds/valley/clouds_mg_2.png", true, "coulds_mg2");
    ResourceManager::LoadTexture("textures/backgrounds/valley/clouds_mg_1.png", true, "coulds_mg1");
    ResourceManager::LoadTexture("textures/backgrounds/trees.png", true, "trees");
    ResourceManager::LoadTexture("textures/backgrounds/Hills_Layer_05.png", true, "grass");

    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("sky_lightened"), glm::vec2(0.0f, 0.0f), 0.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("clouds_bg"), glm::vec2(0.0f, 0.0f), 2.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("mountains"), glm::vec2(0.0f, 0.0f), 5.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("coulds_mg3"), glm::vec2(0.0f, 0.0f), 10.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("coulds_mg2"), glm::vec2(0.0f, 0.0f), 16.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("coulds_mg1"), glm::vec2(0.0f, 0.0f), 28.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("trees"), glm::vec2(0.0f, 0.0f), 50.0f });
    BackgroundLayers.push_back(BackgroundLayer{ ResourceManager::GetTexture("grass"), glm::vec2(0.0f, 0.0f), 50.0f });

    ResourceManager::LoadTexture("textures/backgrounds/valley/cloud_lonely.png", true, "cloud_loney");
    ResourceManager::LoadTexture("textures/sprites/Owlet_Monster.png", true, "monster");
    ResourceManager::LoadTexture("textures/sprites/Moon_Phase_1.png", true, "moon");
    /*ResourceManager::LoadTexture("textures/backgrounds/background.jpg", false, "background");
    ResourceManager::LoadTexture("textures/sprites/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("textures/sprites/block.png", false, "block");
    ResourceManager::LoadTexture("textures/sprites/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("textures/sprites/paddle.png", true, "paddle");*/
    // load levels
    /*GameLevel one; one.Load("src/utils/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("src/utils/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("src/utils/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("src/utils/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;*/

    // load player
    glm::vec2 playerPos = glm::vec2(
        this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
        this->Height - PLAYER_SIZE.y - this->Height * 0.06f
    );
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("monster"));

    //Sun->Color = glm::vec3(0.5f, 0.61f, 0.97f); -- lepa plava boja

    this->LastCloud = glfwGetTime() - MIN_TIME_DIFF - 1;
    RotationRadius = this->Height / 2.0f ;

    glm::vec2 sunPos = glm::vec2(
        this->Width / 2.0f ,
        this->Height - this->Height + SUN_SIZE.y * 2.0f
    );
    Sun = new GameObject();
    Sun->Position = sunPos;
    Sun->Size = SUN_SIZE;
    Sun->Color = glm::vec3(1.0f, 0.917f, 0.502f);

    RotationCenter = glm::vec2(this->Width / 2.0f, Sun->Position.y + RotationRadius);

    glm::vec2 moonPos = glm::vec2(
        this->Width / 2.0f - SUN_SIZE.x,
        this->Height + SUN_SIZE.y * 2.0f
    );
    Moon = new GameObject(moonPos, SUN_SIZE * 2.0f, ResourceManager::GetTexture("moon"));

    for (int i = 0; i < 100; ++i) {
        glm::vec2 starPos = glm::vec2(getRandomInt(0, Width), getRandomInt(0, Height / 2));
        GameObject star = GameObject();
        star.Alpha = 0.0f;
        star.Position = starPos;
        star.Color = glm::vec3(1.0f);
        this->Stars.push_back(star);
    }

    Tint = new GameObject();
    Tint->Size = glm::vec2(
        this->Width,
        this->Height
    );
    Tint->Position = glm::vec2(0.0f);
    Tint->Color = glm::vec3(1.0f, 0.0f, 0.0f);
}

void Game::Update(float dt)
{
    float time = glfwGetTime();
    float pulse = (sin(time * PULSE) + 1.0f) / 2.0f;
    Sun->Size = SUN_SIZE + glm::vec2(sin(time * PULSE));
    Sun->Color = glm::vec3(
        1.0f,                              
        0.917f * (1.0f - pulse) + 0.5f * pulse,
        0.502f * (1.0f - pulse) + 0.3f * pulse 
    );

    if (time - this->LastCloud >= MIN_TIME_DIFF) {
        this->LastCloud = time;

        glm::vec2 cloudSize = glm::vec2(
            getRandomInt(80, 100),
            getRandomInt(50, 80)
        );
        glm::vec2 cloudPos = glm::vec2(
            this->Width + cloudSize.x,
            getRandomInt(0.0f, this->Height / 3.0f - cloudSize.y)
        );

        GameObject NewCloud = GameObject(cloudPos, cloudSize, ResourceManager::GetTexture("cloud_loney"));
        Clouds.push_back(NewCloud);
    }

    float cloud_velocity = CLOUD_VELOCITY * dt;
    for (auto it = this->Clouds.begin(); it != this->Clouds.end();) {
        it->Position.x -= cloud_velocity;

        if (it->Position.x <= -it->Size.x) {
            it = this->Clouds.erase(it);
            //std::cout << "Obrisan\n" << std::endl;
        }
        else {
            ++it;
        }
    }

    if (SunMoving) {
        // Poveæajte ugao (pretvaranje stepena u radijane)
        SunAngle += SunSpeed * dt * (3.14159f / 180.0f);

        // Održavajte ugao u opsegu [0, 2]
        if (SunAngle >= 2.0f * 3.14159f)
            SunAngle -= 2.0f * 3.14159f;

        // Izraèunajte novu poziciju
        Sun->Position.x = RotationCenter.x - RotationRadius * sin(SunAngle);
        Sun->Position.y = RotationCenter.y - RotationRadius * cos(SunAngle);

        Moon->Position.x = RotationCenter.x - (RotationRadius + SUN_SIZE.x) * sin(SunAngle + 3.14159f);
        Moon->Position.y = RotationCenter.y - (RotationRadius + SUN_SIZE.y) * cos(SunAngle + 3.14159f);

        const float tolerance = 0.01f;

        // Check if the Moon's Y position is near the highest point (with tolerance)
        if (Moon->Position.y <= RotationCenter.y - (RotationRadius + SUN_SIZE.y) + tolerance) {
            // Stop the Sun and Moon from moving when the Moon is at the top
            SunMoving = false;
        }

        float normalizedSunAngle = SunAngle / (2.0f * 3.14159f); // Normalize the sun angle

        float cloudTransitionFactor = 0.0f;

        if (normalizedSunAngle >= 0.0f && normalizedSunAngle < 0.25f) {
            // Transition from Day to Sunset
            float t = normalizedSunAngle / 0.25f;
            SkyColor = glm::mix(SkyColorDay, SkyColorSunset, t);
            cloudTransitionFactor = 0.0f; // Clouds transitioning to gray and semi-transparent
        }
        else if (normalizedSunAngle >= 0.25f && normalizedSunAngle < 0.5f) {
            // Transition from Sunset to Night
            float t = (normalizedSunAngle - 0.25f) / 0.25f;
            SkyColor = glm::mix(SkyColorSunset, SkyColorNight, t);
            cloudTransitionFactor = t; // Clouds are fully gray and semi-transparent
        }
        else if (normalizedSunAngle >= 0.5f && normalizedSunAngle < 0.75f) {
            // Transition from Night to Morning
            float t = (normalizedSunAngle - 0.5f) / 0.25f;
            SkyColor = glm::mix(SkyColorNight, SkyColorMorning, t);
            cloudTransitionFactor = 1.0f - t; // Clouds transitioning back from gray to full color
        }
        else {
            // Transition from Morning to Day
            float t = (normalizedSunAngle - 0.75f) / 0.25f;
            SkyColor = glm::mix(SkyColorMorning, SkyColorDay, t);
            cloudTransitionFactor = 0.0f; // Clouds are fully visible and opaque
        }

        isNight = (normalizedSunAngle >= 0.30f && normalizedSunAngle < 0.75f) ? true : false;

        CloudColor = glm::mix(glm::vec3(1.0f), glm::vec3(0.5f), cloudTransitionFactor); // Transition from white to gray
        CloudAlpha = glm::mix(1.0f, 0.5f, cloudTransitionFactor); // Semi-transparent
    }

    if (MoonClicked)
    {
        redProgress += !RemoveRed ? 0.005 : -0.005; // Adjust speed as needed
        if (redProgress >= 1.0f) {
            redProgress = 1.0f; // Clamp to max
            MoonClicked = false; // Stop animation
            RemoveRed = true;
        }
        else if (redProgress < 0.0f) {
            redProgress = 0.0f; // Clamp to max
            MoonClicked = false; // Stop animation
            RemoveRed = false;
        }
        else {
            ResourceManager::GetShader("moon").Use().SetFloat("redProgress", redProgress);
        }
    }

    for (auto& star : Stars) {
        star.Alpha = 1.0f - CloudAlpha;
        star.Size = glm::vec2((sin(glfwGetTime() * 2.0f + getRandomInt(1, 100)) + 1.0f) * 0.5f);
    }
}

void Game::ProcessInput(float dt, float MouseX, float MouseY)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A]) {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;

            // Update background layers
            for (auto& layer : BackgroundLayers) {
                layer.position.x -= layer.velocity * dt;

                // Wrap the position when it goes out of bounds
                if (layer.position.x <= -static_cast<float>(this->Width)) {
                    layer.position.x += static_cast<float>(this->Width);
                }
            }
        }

        if (this->Keys[GLFW_KEY_D]) {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;

            // Update background layers
            for (auto& layer : BackgroundLayers) {
                layer.position.x += layer.velocity * dt;

                // Wrap the position when it goes out of bounds
                if (layer.position.x >= static_cast<float>(this->Width)) {
                    layer.position.x -= static_cast<float>(this->Width);
                }
            }
        }

        if (this->Keys[GLFW_KEY_N] && !(Moon->Position.y <= RotationCenter.y - (RotationRadius + SUN_SIZE.y) + 0.01)) {
            SunMoving = true;
        }

        if (this->MouseInput[GLFW_MOUSE_BUTTON_LEFT] && Moon->IsClicked(MouseX, MouseY) 
            && (redProgress == 0.0f || redProgress == 1.0f)) {
            MoonClicked = !MoonClicked;
            if (RemoveRed) {
                ResourceManager::GetShader("moon").Use().SetInteger("direction", -1);
            }
            else {
                ResourceManager::GetShader("moon").Use().SetInteger("direction", 1);
            }
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        glm::vec2 texRepeat(1.5f, 1.0f);
        glm::vec2 texOffset(BackgroundLayers[0].position.x / this->Width, 0.0f);
        Renderer->DrawSprite(BackgroundLayers[0].texture,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 1.0f,texOffset, texRepeat, 0.0, SkyColor, false);


        for (GameObject star : Stars) {
            star.Draw(*CircleRenderer);
        }

        // TODO Sunce i mesec
        //std::cout << "Rendering Sun at position: " << Sun->Position.x << ", " << Sun->Position.y << std::endl;
        Sun->Draw(*CircleRenderer);
        //printf("%f\n", redProgress);
        ResourceManager::GetShader("moon").Use().SetInteger("hasTexture", 1);
        Moon->Draw(*MoonRenderer);

        for (size_t i = 1; i < BackgroundLayers.size() - 2; ++i)
        {
            auto layer = BackgroundLayers[i];
            glm::vec2 texOffset(layer.position.x / this->Width, 0.0f);

            Renderer->DrawSprite(layer.texture,
                glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height * 0.9), 1.0f,
                texOffset, texRepeat);
        }

        texRepeat = glm::vec2(3.0f, 1.0f);
        texOffset = glm::vec2(BackgroundLayers[BackgroundLayers.size() - 2].position.x / this->Width, 0.0f);
        Renderer->DrawSprite(BackgroundLayers[BackgroundLayers.size() - 2].texture,
            glm::vec2(0.0f, 0.0f + this->Height * 0.1), glm::vec2(this->Width, this->Height * 0.9), 1.0f,texOffset, texRepeat);

        // Draw player and other objects
        Player->Draw(*Renderer);

        texRepeat = glm::vec2(3.0f, 1.0f);
        texOffset = glm::vec2(BackgroundLayers[BackgroundLayers.size() - 1].position.x / this->Width, 0.0f);
        Renderer->DrawSprite(BackgroundLayers[BackgroundLayers.size() - 1].texture,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height),1.0f, texOffset, texRepeat);

        for (GameObject& cloud : Clouds) {
            cloud.Color = CloudColor;
            cloud.Alpha = CloudAlpha;
            cloud.Draw(*Renderer);
        }

        ResourceManager::GetShader("moon").Use().SetInteger("hasTexture", -1);
        Tint->Draw(*MoonRenderer);
    }
}

int getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}