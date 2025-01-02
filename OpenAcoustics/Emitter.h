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
	float ampl;
	float phase;
	float radius;

public:
	Emitter(vec3 position = vec3(), vec3 color = vec3(), float frequency = 0, float amplitude = 0, float phased = 0, float rad = 0) {
		setPos(position);
		setCol(color);
		setFrequency(frequency);
		setAmpl(amplitude);
		setPhase(phased);
		setRadius(rad);
	}

	void setPos(vec3 pos) {
		this->pos = pos;
	}
	void setCol(vec3 col) {
		this->col = col;
	}
	void setCol(float r) {
		this->col = vec3(r,r,r);
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
	void setAmpl(float ampl) {
		this->ampl = ampl;
	}
	void setRadius(float radius) {
		this->radius = radius;
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
	float getAmplitude() {
		return ampl;
	}
	float getRadius() {
		return radius;
	}
	float getPhase() {
		return phase;
	}

	// Position differentiation
	bool operator == (vector<Emitter> t) const {
			for (Emitter l : t) {
				if (pos == l.getPos())
					return true;
			}

		return false;
	}
	bool operator == (Emitter t) const {
		bool a = true; (t.getPos() == pos ? a : 0);
		return a;
	}
};