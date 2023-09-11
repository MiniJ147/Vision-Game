#pragma once
#include <GLFW/glfw3.h>

inline bool check_key_press(GLFWwindow* window, int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS ? true : false;

}
