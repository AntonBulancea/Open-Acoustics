#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Emitter {

private:
	vec3 pos;
	float freq;
	float phase;

public:
	void setPos(vec3 pos) {
		this->pos = pos;
	}
	void setFrequency(float freq) {
		this->freq = freq;
	}
	void setPhase(float phase) {
		this->phase = phase;
	}

	vec3 getPos() {
		return pos;
	}
	float getFrequency() {
		return freq;
	}
	float getPhase() {
		return phase;
	}
};