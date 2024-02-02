// Curious Orbital Toy
// Malhar Palkar
#include <curious-orbital-toy.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Array of orbital bodies
body_t scene[COT_BODYCOUNT];

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* gWindow = glfwCreateWindow(800, 600, "Curious Orbital Toy", NULL, NULL);
    if (!gWindow)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(gWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(gWindow, framebuffer_size_callback); 

    while(!glfwWindowShouldClose(gWindow))
    {
        glfwSwapBuffers(gWindow);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}