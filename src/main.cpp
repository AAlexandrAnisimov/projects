#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <array>
#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util.h"
#include "Camera.h"
#include "Controller.h"

#include "World\ChunkController.h"
#include "World\World.h"
#include "Gamemaneger.h"

using namespace Util;
int main() {

	Gamemanager.InitializeGLEW(GLFWwindow *window);
	Gamemanager.InitializeGLFW(GLFWwindow *window);
    Gamemanager.Create(Attrib *attrib);
	Gamemanager.Controller(GLFWwindow *window, Camera *camera, ChunkController *ChunkController)

	return 0;
}
