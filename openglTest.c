
#include <stdlib.h>
#include <stdio.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

int main()
{
	GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}