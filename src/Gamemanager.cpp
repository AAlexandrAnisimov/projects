#include "Gamemaneger.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <array>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
  ' '   '           '
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>


static int Gamemanager::InitializeGLEW(GLFWwindow *window)
{
    	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}
static void Controller(GLFWwindow *window, Camera *camera, ChunkController *chunkController): window(window), camera(camera), chunkController(chunkController){
    Camera camera(window, &attrib, 60.0f, 1280.0f, 720.0f, .1f, 700.0f);
	ChunkController c(&camera);
	Controller controller(window, &camera, &c);

	World world(&c, &camera, &controller);

	glClearColor(0.3f, 0.77f, 1.0f, 0.0f);

	do {
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//update camera
		camera.update(&attrib);

		//drawing
		//glDrawElements(GL_TRIANGLES, NELEMS(elements), GL_UNSIGNED_INT, 0);
		c.draw(&attrib);

		world.update();

		//update controller
		controller.update(&attrib);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwWindowShouldClose(window) == 0);

	c.save_data(world.filename);

	glfwTerminate();
}
static int Gamemanager::InitializeGLFW(GLFWwindow *window)

{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

	int xpos = 50;
	int ypos = 50;
	if (monitorCount >= 2) {
		//xpos = -1300;
		xpos = 1800;
		ypos = -150;
	}
	int width = 1280;
	int height = 720;
	window = glfwCreateWindow(width, height, "xx", NULL, NULL);
	glfwSetWindowMonitor(window, NULL, xpos, ypos, width, height, 0);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
}


static void Gamemanager::Create(Attrib *attrib) {
    	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	Attrib attrib = { 0 };

	GLuint shaderProgram = load_shader("../../../shaders/vertex1.glsl", "../../../shaders/fragment1.glsl");

	attrib.program = shaderProgram;

	shaderProgram = load_shader("../../../shaders/vertex2.glsl", "../../../shaders/fragment1.glsl");

	attrib.program2 = shaderProgram;

	shaderProgram = load_shader("../../../shaders/vertex3.glsl", "../../../shaders/fragment1.glsl");

	attrib.program3 = shaderProgram;



	attrib.block_tex = load_image("../../../asset/gimp/blocks4.png");
	attrib.crosshair = load_image("../../../asset/gimp/crosshair.png");
	attrib.lower_inventory = load_image("../../../asset/gimp/lower_inventory.png");
	attrib.lower_inventory_selector = load_image("../../../asset/gimp/lower_inventory_selector.png");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
