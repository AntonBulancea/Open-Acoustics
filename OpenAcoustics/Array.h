#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Shader.h"
#include "Particle.h"
#include "Simulation.h"
#include "Slice.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class Array {

private:
	const float cm = 0.1f;

	int rowSize = 10;
	int collumnSize = 10;
	int layerSize = 1;

	float space = 0.1;
	float transSize = 1;
	float layerHeight = 1.0f;

	float frequency = 40000;

	vector<Emitter> emitters;
	vector<Particle> particles;
	vector<Slice> slices;

public:
	void generateArray(vec3 startColor) {
		vec3 pos;
		Emitter emi;

		for (int k = 0; k < layerSize; k++) {
			for (int i = 0; i < collumnSize; i++) {
				for (int j = 0; j < rowSize; j++) {

					float xVal = (j * cm) * (transSize + space);
					float yVal = (i * cm) * (transSize + space);
					float zVal = layerHeight * k;

					pos = vec3(xVal, zVal, yVal);
					emi = Emitter();

					emi.setPos(pos);
					emi.setFrequency(frequency);

					emitters.push_back(emi);
				}
			}
		}

	}
	void generateParticle(vec3 pos, int num = 1) {
		Particle par;
		for (int i = 0; i < num; i++) {
			par.setPos(pos);
			particles.push_back(par);
		}
	}

	void DrawArray(Model model, Shader shader, mat4 mod) {
		vec3 pos;

		for (Emitter emi : emitters) {
			pos = emi.getPos();

			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);
			
			translation = translate(translation,pos);
			scaling = scale(scaling, vec3(cm/1.8, cm/1.8, cm/1.8));
			AssignPhasesToArray(GetParticle(0));

			shader.setVec3("color", emi.getCol());
			shader.setMat4("model", mod*translation*scaling);
			shader.use();

			model.Draw(shader);
		}
		}
	void DrawParticleArray(Model model, Shader shader, mat4 mod) {
		vec3 pos;

		for (Particle par : particles) {
			pos = par.getPos();

			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);

			translation = translate(translation, pos);
			scaling = scale(scaling, vec3(cm / 8, cm / 8, cm / 8));

			shader.setMat4("model", mod * translation * scaling);
			shader.setVec3("color", par.getColor());

			model.Draw(shader);
		}
	}
	void DrawSliceArray(Model model, Shader shader, mat4 mod){
		vec3 pos;

		for (Slice sli : slices) {
			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);

			translation = translate(translation, pos);
			scaling = scale(scaling, vec3(cm / 8, cm / 8, cm / 8));

			shader.setMat4("model", mod * translation * scaling);

			model.Draw(shader);
		}
	}

	void AssignPhasesToArray(Particle par) {
		vec3 pos = par.getPos();
		Simulation sim;
		for (Emitter &emi : emitters) {
			emi.setPhase(sim.CalculatePhase(emi,par));
			emi.setColFromPhase();
		}
	}

	Particle &GetParticle(int pointer) {
		return particles.at(pointer);
	}
	Emitter &GetEmitter(int pointer) {
		return emitters.at(pointer);
	}

	vec3 FindArrayCenter(float height) {
		int centerCoord = collumnSize * (rowSize / 2) + (collumnSize / 2);
		vec3 pos = emitters.at(centerCoord).getPos();
		return vec3(pos.x,pos.y+height,pos.z);
	}
};