#ifndef OS_H
#define OS_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "gio_camera.h"

#include "application.h"

#include "gio_shader.h"
#include "gio_model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

namespace OS
{
    // window settings
    extern unsigned int display_width;
    extern unsigned int display_height;

    // camera
    extern Camera camera;
    extern float lastX;
    extern float lastY;
    extern bool firstMouse;

    // timing
    extern float deltaTime;
    extern float lastFrame;

    extern glm::vec3 mouse_position;

    extern GLFWwindow *init();

    extern void update(GLFWwindow *window);

    extern void render(Shader& shaderGeometryPass, Shader& shaderLightingPass, Shader& shaderLightBox);
}

#endif