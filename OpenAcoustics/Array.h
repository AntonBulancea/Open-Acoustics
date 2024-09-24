#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

#include "Model.h"
#include "Shader.h"
#include "Particle.h"
#include "Simulation.h"
#include "SerialConnection.h"
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
	void generateArray(vec3 startColor, float zHeight) {
		vec3 pos;
		Emitter emi;
		layerHeight = zHeight;

		for (int k = 0; k < layerSize; k++) {
			for (int i = 0; i < collumnSize; i++) {
				for (int j = 0; j < rowSize; j++) {

					float xVal = (j * cm) * (transSize + space);
					float yVal = (i * cm) * (transSize + space);
					float zVal = layerHeight;

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

	void DrawArray(Model model, Shader shader, mat4 mod, Emitter selected, SerialConnection &ser){
		vec3 pos;
		AssignPhasesToArray(GetParticle(0));

		for (Emitter emi : emitters) {
			//Drawing the Emitters

			pos = emi.getPos();

			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);
			
			translation = translate(translation,pos);
			scaling = scale(scaling, vec3(cm/1.8, cm/1.8, cm/1.8));

			//If drawing the selected emitter
			if (selected.getPos() == emi.getPos())
				emi.setCol(vec3(255, 0, 0));

			shader.setVec3("color", emi.getCol());
			shader.setMat4("model", mod*translation*scaling);
			shader.use();

			//Sending Info to Serial Port
			
			//int ph = emi.getPhase();
			//ser.SendPhase(ph);

			model.Draw(shader);
		}
		}
	void DrawParticleArray(Model model, Shader shader, mat4 mod) {
		vec3 pos;
		vec3 shad;

		for (Particle par : particles) {
			pos = par.getPos();
			shad = par.getPosShad();

			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);

			translation = translate(translation, pos);
			scaling = scale(scaling, vec3(cm / 8, cm / 8, cm / 8));

			shader.setMat4("model", mod * translation * scaling);
			shader.setVec3("color", par.getColor());

			model.Draw(shader);
			
			mat4 translatio1n = mat4(1.0f);
			translatio1n = translate(translatio1n, shad);
			shader.setVec3("color", par.getColorShad());
			shader.setMat4("model", mod * translatio1n * scaling);

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

	void SetParticlePos(int n, vec3 pos) {
		particles.at(n).setPos(pos);
	}
	void SetShadowPos(int n, vec3 pos) {
		particles.at(n).setPosShad(pos);
	}
	void AssignPhasesToArray(Particle par) {
		vec3 pos = par.getPos();
		Simulation sim;
		for (Emitter &emi : emitters) {
			emi.setPhase(sim.CalculatePhase(emi,par));
			emi.setColFromPhase();
		}
	}

	int getColSize() {
		return (collumnSize * cm) + (space*cm*collumnSize);
	}
	int getRowSize() {
		return (rowSize * cm) + (space * cm * rowSize);
	}

	Particle &GetParticle(int pointer) {
		return particles.at(pointer);
	}
	Emitter &GetEmitter(int pointer) {
		return emitters.at(pointer);
	}
	Emitter &GetEmitter(vec3 pos) {
		Emitter posEmi;
		float error = numeric_limits<float>::max(); // X & Z

		for (Emitter& emi : emitters) {
			float score = abs(pos.x - (emi.getPos().x)) + abs(pos.z - (emi.getPos().z));

			if(score < error){
				error = score;
				posEmi = emi;
			}
		}

		return posEmi;
	}
	vector<int> &GetPhases() {
		vector<int> temp;

		for (Emitter &emi : emitters)
			temp.push_back(emi.getPhase());

		return temp;
	}
	vec3 FindArrayCenter(float height) {
		int centerCoord = collumnSize * (rowSize / 2) + (collumnSize / 2);
		vec3 pos = emitters.at(centerCoord).getPos();
		return vec3(pos.x,pos.y+height,pos.z);
	}
	vec3 FindArrayBegining() {
		return emitters.at(0).getPos();
	}
};