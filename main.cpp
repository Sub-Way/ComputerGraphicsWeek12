/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include "math.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include "Geometry/TriangularSurface/StaticTriangularSurface.h"
#include "GL2/GL2_Object.h"
#include "GL2/GL2_World.h"
#include "GL2/GL2_Light.h"
#include "GL2/GL2_Material.h"

int width_window = 640;
int height_window = 480;

GL2_World gl_world;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gl_world.camera_.UpdateDolly(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{ 
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		gl_world.camera_.StartMouseRotation(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		gl_world.camera_.EndMouseRotation(xpos, ypos);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		gl_world.camera_.StartMousePan(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		gl_world.camera_.EndMousePan(xpos, ypos);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << xpos << " " << ypos << std::endl;
	
	gl_world.camera_.ProcessMouseMotion(xpos, ypos);	
}

int main(void)
{
	GLFWwindow *window = nullptr;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 32);

	// window resolution
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width_window = mode->width * 0.8f;
	height_window = mode->height * 0.8f;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width_window, height_window, "Hello World", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// callbacks here
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(1, 1, 1, 1); // while background

	printf("%s\n", glGetString(GL_VERSION));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	const float aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glOrtho(-1.2*aspect, 1.2*aspect, -1.2, 1.2, -100.0, 100.0);
	gluLookAt(0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0, 1, 0);//camera takes care of it

	const float zNear = 0.001, zFar = 100.0, fov = 45.0;			// UI
	gl_world.camera_.Resize(width, height, fov, zNear, zFar);
	gl_world.initShaders();

//	glEnable(GL_MULTISAMPLE);

	StaticTriangularSurface surface;
	//surface.readObj("BuddhaSculpture.obj", true, true);
	surface.readObj("Dark_Star_Toad.obj", true, true);
	surface.advanceOneSubdivisionStep(0);
	surface.advanceOneSubdivisionStep(1);
	surface.advanceOneSubdivisionStep(2);


	GL2_Object gl_obj;
	gl_obj.initPhongSurface(surface);
	
	//gl_obj.mat_.setRed();
	gl_obj.mat_.setGold();

	// depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);

	glLoadIdentity();

	GL2_Light light;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		gl_world.camera_.ContinueRotation();

		glm::mat4 vp = gl_world.camera_.GetWorldViewMatrix();

		// don't need to update if there is no change
		// gl_obj.updatePhongSurface(surface); 

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gl_world.shaders_.program_id_);

		gl_world.shaders_.sendUniform(vp, "mvp");

		//gl_obj.drawPhongSurface();
		gl_obj.applyLighting(light);
		//gl_obj.drawWithShader(gl_world.shaders_);
		gl_obj.drawPhongSurface();
		
		glUseProgram(0);

		// Old-style rendering

		glLoadMatrixf(&vp[0][0]);

		glLineWidth(2);
		gl_world.drawAxes();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	glfwTerminate();

	return 0;
}

