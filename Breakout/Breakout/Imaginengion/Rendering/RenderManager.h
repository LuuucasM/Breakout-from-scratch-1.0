#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bitset>

enum RenderInterfaceType{
    OPENGL
};

class RenderManager{
private:
    RenderInterfaceType GraphicInterface;
    GLFWwindow *Window;
public:
    int WINDOW_WIDTH = 0;
    int WINDOW_HEIGHT = 0;
    const char *WINDOW_NAME;
    std::bitset<1024> Keys;

    RenderManager(RenderInterfaceType type, int window_width, int window_height, const char *window_name);
    ~RenderManager();
    int Init();
    bool WindowShouldClose();
    void ProcessInput();
    void SwapBuffers();
    void PollEvents();
};