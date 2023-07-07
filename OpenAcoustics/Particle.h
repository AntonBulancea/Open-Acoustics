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
	float density = 1;
public:
	Particle(vec3 color = vec3(0, 0.502, 0.392)){
		this->color = color;
	}

	void setColor(vec3 col) {
		this->color = col;
	}
	void setPos(vec3 pos) {
		this->pos = pos;
	}
	void setDensity(float freq) {
		this->density = freq;
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
};