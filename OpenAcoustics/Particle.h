#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

using namespace glm;
using namespace std;

class Particle {
private:
	vec3 pos;
	vec3 color;

	vec3 shadowCol;
	vec3 shadowPos;

	float density = 1;
public:
	Particle(vec3 color = vec3(0, 0.502, 0.392),vec3 shadowCol = vec3(0, 0.502, 0.392)){
		this->color = color;
		this->shadowCol = shadowCol;
	}

	//Main Particle
	void setColor(vec3 col) {
		this->color = col;
	}
	void setPos(vec3 pos) {
		this->pos = pos;
	}
	void setDensity(float density) {
		this->density = density;
	}

	float getDensity() {
		return density;
	}
	vec3 getPos() {
		return pos;
	}
	vec3 getColor() {
		return color;
	}

	//Shadow Particle
	void setColorShad(vec3 col) {
		shadowCol = col;
	}
	void setPosShad(vec3 pos) {
		shadowPos = pos;
	}
	
	vec3 getPosShad() {
		return shadowPos;
	}
	vec3 getColorShad() {
		return shadowCol;
	}
};