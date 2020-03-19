#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

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
using namespace Util;
class Gamemanager
{
    public static Gamemanager instance = null;
    private:
        static GLFWwindow* window;
        static	Camera *camera;
        static ChunkController *chunkController;

        Gamemanager() {}
        Gamemanager(const Gamamanager&);
        Gamemanager& operator=(Gamemanager&);
    public:
        static void Awake()
        {
            //Check if instance already exists
            if (instance == null)

                //if not, set instance to this
                instance = this;

            //If instance already exists and it's not this:
            else if (instance != this)

                //Then destroy this. This enforces our singleton pattern, meaning there can only ever be one instance of a GameManager.
                ~Gamemanager();
        }
        static int InitializeGLFW(GLFWwindow *window);
        static int InitializeGLEW(GLFWwindow *window);
        static void Create(Attrib *attrib);
        static void Controller(GLFWwindow *window, Camera *camera, ChunkController *chunkController);
};


#endif
