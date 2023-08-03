#include <glad.h>
#include <glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Model.h"
#include "Emitter.h"
#include "Array.h"
#include "CameraInstance.h"

//Functions Call
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void clearColors();

GLFWwindow* initializeWindow();

//OpenGl Values
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int VAO, VBO;

//OpenCv Values
bool showCameraVid = false;

//Time Values
float deltaTime = 0.0f;
float lastFrame = 0.0f;

using namespace glm;
using namespace std;

//Camera Values
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -2.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float currentFrame;

bool firstMouse = true;
float yaw_ = -90.0f;
float pitch_ = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 90.0f;

int h = 0.0f;

//Emitter Colors
vec3 emiColor = vec3(0.337, 0.537, 0.859);
vec3 parColor = vec3(0.0, 0.3, 0.0);
vec3 particlePos = vec3(0.0f, 0.0f, 0.0f);

int main()
{
	//Classes & Variables Ini
	GLFWwindow* window = initializeWindow();
	
	Shader emiShader("emiVertex.glsl", "emiFragment.glsl");
	Shader parShader("parVertex.glsl", "parFragment.glsl");
	Shader sliceShader("sliceVertex.glsl", "sliceFragment.glsl");

	Model emitter = Model("emitter.obj");
	Model particle = Model("particle.obj");
	Model slice = Model("slice.obj");

	//Create a Window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set Up Matrix
	mat4 view = mat4(1.0f);
	mat4 projection = mat4(1.0f);
	
	mat4 emiModel = mat4(1.0f);
	mat4 parModel = mat4(1.0f);

	Array arr = Array();
	arr.generateArray(emiColor);
	arr.generateParticle(particlePos = arr.FindArrayCenter(.5f));

	CameraInstance instance = CameraInstance(0);
	
	//Update Loop
	while (!glfwWindowShouldClose(window))
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Calculate DeltaTime
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);
		clearColors();

		// Rendering
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = perspective(radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		emiShader.setMat4("view", view);
		emiShader.setMat4("projection", projection);

		arr.DrawArray(emitter, emiShader, emiModel);
		arr.GetParticle(0).setPos(particlePos);
		arr.DrawParticleArray(particle, parShader, parModel);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// OpenCV update
		showCameraVid = instance.UpdateView(showCameraVid);
		cout << showCameraVid;
	}

	glfwTerminate();
	return 0;
}

GLFWwindow* initializeWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a Window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Open Acoustics", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	return window;
}

void clearColors() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void processInput(GLFWwindow* window)
{
	const float cameraspeed = 3.0f * deltaTime;
	const float partspeed = 0.5f * deltaTime;

	if (glfwGetKey(window, GLFW_RAW_MOUSE_MOTION) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraspeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraspeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraspeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraspeed;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		particlePos += cameraspeed * vec3(0.0f, 0.0f, -2.0f) /= 5;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		particlePos -= cameraspeed * vec3(0.0f, 0.0f, -2.0f) /= 5;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		particlePos -= normalize(cross(vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f))) * partspeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		particlePos += normalize(cross(vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f))) * partspeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		particlePos += vec3(0.0f, 1.0f, 0.0f) * partspeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		particlePos += vec3(0.0f, -1.0f, 0.0f) * partspeed;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		showCameraVid = true;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		showCameraVid = false;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= yoffset * 5;

	if (fov < 0)
		fov = 1;
	if (fov > 120)
		fov = 119;
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw_ += xoffset;
	pitch_ += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch_ > 89.0f)
		pitch_ = 89.0f;
	if (pitch_ < -89.0f)
		pitch_ = -89.0f;

	vec3 front;
	front.x = cos(radians(yaw_)) * cos(radians(pitch_));
	front.y = sin(radians(pitch_));
	front.z = sin(radians(yaw_)) * cos(radians(pitch_));
	cameraFront = normalize(front);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}