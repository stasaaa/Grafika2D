#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H 

#include "core/Game.h"
#include "core/resource_manager.h"
#include "entities/characters.h"

const int TARGET_FPS = 60;
const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

unsigned int SCREEN_WIDTH = 1500;
unsigned int SCREEN_HEIGHT = 700;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
	if (!glfwInit())
	{
		std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Learning OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Could not create window :(" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW nije mogao da se ucita! :'(\n";
		glfwTerminate();
		return 3;
	}

    // freetype
    Characters* TextCharacters = new Characters();

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/Jersey_10/Jersey10-Regular.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned int c = 0; c < 2048; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            TextCharacters->AllCharacters.insert(std::pair<unsigned int, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // other functions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Dovde je samo ekran
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // initialize game
    // ---------------
    Breakout.Init(*TextCharacters);

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    int frame = 0;

    while (!glfwWindowShouldClose(window)) {
        //double frameStart = glfwGetTime();
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        //std::cout << "Frame: " << frame++ << "\tDelat time: " << deltaTime << std::endl;

        if (deltaTime < TARGET_FRAME_TIME) {
            //std::cout << "Delta time: " << deltaTime << "\tSleep for: " << static_cast<int>((TARGET_FRAME_TIME - deltaTime) * 1000) << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((TARGET_FRAME_TIME - deltaTime) * 1000)));
        }

        double MouseX, MouseY;
        glfwGetCursorPos(window, &MouseX, &MouseY);
        Breakout.ProcessInput(deltaTime, MouseX, MouseY);

        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Update(deltaTime);
        Breakout.Render();

        // send new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastFrame = currentFrame;
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();
    delete TextCharacters;
    // zavrsavanje programa
    glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Breakout.UpdatePositions(width, height);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width),
        static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    for (const auto& shaderPair : ResourceManager::Shaders) {
        ResourceManager::GetShader(shaderPair.first).Use().SetMatrix4("projection", projection);
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) {
            Breakout.Keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            Breakout.Keys[key] = false;
        }
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= 0 && button < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.MouseInput[button] = true;
        else if (action == GLFW_RELEASE)
            Breakout.MouseInput[button] = false;
    }
}
