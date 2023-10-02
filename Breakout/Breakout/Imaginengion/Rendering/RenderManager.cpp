

#include "RenderManager.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    RenderManager* manager = (RenderManager*)glfwGetWindowUserPointer(window);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            manager->Keys.set(key);
        else if (action == GLFW_RELEASE)
            manager->Keys.reset(key);
    }
}

RenderManager::RenderManager(RenderInterfaceType type, int window_width, int window_height, const char *window_name) {
    GraphicInterface = type;
    WINDOW_WIDTH = window_width;
    WINDOW_HEIGHT = window_height;
    WINDOW_NAME = window_name;
}

RenderManager::~RenderManager(){

}

int RenderManager::Init(){
    switch(GraphicInterface){
        case RenderInterfaceType::OPENGL:
            glfwInit();
	        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
            if (Window == NULL) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
            }
            glfwMakeContextCurrent(Window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "Failed to initialize glad" << std::endl;
                return -1;
            }

            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glfwSetWindowUserPointer(Window, (void*)this);
            glfwSetKeyCallback(Window, key_callback);
            glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    }
    Keys.reset();
    return 0;
}

void RenderManager::SetWindowShouldClose(bool val) {
    glfwSetWindowShouldClose(Window, val);
}

bool RenderManager::GetWindowShouldClose() {
    return glfwWindowShouldClose(Window);
}

void RenderManager::ClearBuffer() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderManager::SwapBuffers() {
    glfwSwapBuffers(Window);
}

void RenderManager::PollEvents() {
    glfwPollEvents();
}