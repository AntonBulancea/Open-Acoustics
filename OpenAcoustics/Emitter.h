#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Emitter {

private:
	vec3 pos;
	vec3 col;
	float freq;
	float phase;

public:
	void setPos(vec3 pos) {
		this->pos = pos;
	}
	void setCol(vec3 col) {
		this->col = col;
	}
	void setColFromPhase() {
		float colPhase = getPhase() / 2;
		setCol(vec3(colPhase,colPhase,colPhase));
	}
	void setFrequency(float freq) {
		this->freq = freq;
	}
	void setPhase(float phase) {
		this->phase = phase;
	}
	void DebugInfo() {
		cout << "Emitter Info:" << endl;
		cout << "Freq: " << freq << endl;
		cout << "Phase: " << phase << endl << endl;
	}

	vec3 getCol() {
		return col;
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