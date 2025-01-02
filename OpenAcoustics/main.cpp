#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<stb/stb_image.h>

#include <iostream>
#include <vector>
#include <math.h>

#include "Shader.h"
#include "Model.h"
#include "Emitter.h"
#include "Array.h"
#include "CameraInstance.h"
#include "FpgaProtocol.h"
#include "Serial.h"

/*
	Made by Anton B.
*/

//Functions Call
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);
void clearColors();

GLFWwindow* initializeWindow();

//OpenGl Values
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float xpos;
float ypos;

bool isShiftPressed = false;
bool isSliced = false;

//Simulation Values
Array arr = Array();
Serial serial = Serial();
FpgaProtocol prot = FpgaProtocol();

vector<Emitter> selectedEmis;

//Time Values
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

//Emitter Colors
vec3 emiColor = vec3(0.337, 0.537, 0.859);
vec3 parColor = vec3(0, 0.502, 0.392);
vec3 shaColor = vec3(1.0, 0.0, 0.0);

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
	Model slice = Model("sliceVox.obj");

	//Create a Window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set Up Matrix
	mat4 view = mat4(1.0f);
	mat4 projection = mat4(1.0f);

	mat4 emiModel = mat4(1.0f);
	mat4 parModel = mat4(1.0f);
	mat4 sliModel = mat4(1.0f);

	// Array Setup
	arr.generateArray(emiColor);
	arr.generateParticle(particlePos = arr.FindArrayCenter(.2f));
	arr.generateSlice(-(arr.FindArrayCenter(0.1)) + vec3((arr.getCm() / 1.8) * 10, 0, 0), 50, arr.getCm() / 8, arr.getCm() / 4);

	arr.GetParticle(0).setColor(parColor);
	arr.GetParticle(0).setColorShad(shaColor);

	serial.configure();

	float xPo = arr.getColSize();
	float yPo = arr.getColSize();
	float zPo = .2f;

	//Update Loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate DeltaTime
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		arr.GetParticle(0).setPos(particlePos);

		// Input
		processInput(window);
		clearColors();

		// Rendering
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = perspective(radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		emiShader.setMat4("view", view);
		emiShader.setMat4("projection", projection);

		//Draw Arrays
		arr.DrawEmitterArray(emitter, emiShader, emiModel, selectedEmis, serial);
		arr.DrawParticleArray(particle, parShader, parModel);
		if (isSliced)
			arr.DrawSliceArray(slice, sliceShader, sliModel);

		glfwSwapBuffers(window);
		glfwPollEvents();
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
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		isSliced = true;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
		isSliced = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		isShiftPressed = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		isShiftPressed = false;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		cout << serial.read() << endl;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		float emiX, emiY, alpha, beta, r;
		alpha = (radians(pitch_));
		beta = (radians(yaw_));

		//tex: $$ r = tan(\frac{\displaystyle \pi }{2} - {\displaystyle \alpha})z  $$

		//tex: $$ x = cos({\displaystyle \beta})r  $$

		//tex: $$ y = sin({\displaystyle \beta})r  $$

		r = tan(pi_c / 2 - alpha) * cameraPos.y;
		emiX = cos(beta) * r; // x difference from the camera pov
		emiY = sin(beta) * r; // y difference from the camera pov

		emiX = -(emiX)+cameraPos.x; // adding camera pos
		emiY = -(emiY)+cameraPos.z;

		vec3 selectedPos = vec3(emiX, 1, emiY); // searching for the closest emitter

		if (!isShiftPressed)
			selectedEmis.clear();

		selectedEmis.push_back(arr.GetEmitter(selectedPos));
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		selectedEmis.clear();
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		selectedEmis.pop_back();
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
	xpos = static_cast<float>(xposIn);
	ypos = static_cast<float>(yposIn);

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

	if (yaw_ > 360)
		yaw_ = 0;
	if (yaw_ < 0)
		yaw_ = 360;

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