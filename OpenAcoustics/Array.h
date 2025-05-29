#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

#include "Model.h"
#include "Shader.h"
#include "Particle.h"
#include "Slice.h"
#include "Simulation.h"
#include "FpgaProtocol.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class Array {

private:
	const float cm = 0.1f;

	int rowSize = 8;
	int collumnSize = 8;
	int layerSize = 1;

	float space = 0.1;
	float transSize = 1;
	float layerHeight = 1.0f;

	float frequency = 40000;
	float amplitude = 12;
	float rad = transSize * 5;

	float emiPhMin = 100000.0f; // Debug only
	float emiPhMax = -1;

	int emiAssignment = 0;

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

					float xVal = (float)i / 10;
					float yVal = (float)j / 10;
					float zVal = k;

					pos = vec3(xVal, zVal, yVal);
					emi = Emitter();

					emi.setPos(pos);
					emi.setAmpl(amplitude);
					emi.setRadius(rad);
					emi.setFrequency(frequency);
					emi.setPin(-1);

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
			par.setColor(vec3(0.659, 0, 0.031));
		}
	}
	void generateSliceCube(vec3 origin, int dim, float voxSize, float sep) {
		Slice sl;
		Simulation sim;

		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				for (int k = 0; k < dim; k++) {
					Vox v = Vox(vec3(i * sep, j * sep, k * sep) - origin, vec3(1, 1, 1), voxSize, 0.0f);
					sl.setVoxels(v);
				}
			}
		}
		sl.setOrigin(origin);
		slices.push_back(sl);
	}
	void generateSlice(vec3 origin, int dim, float voxSize, float sep) {
		Slice sl;
		Simulation sim;

		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				Vox vox = Vox(vec3(i * sep, j * sep, 0) - origin, vec3(1, 1, 1), voxSize, 0.0f);
				sl.setVoxels(vox);
			}
		}
		sl.setOrigin(origin);
		slices.push_back(sl);
	}

	void DrawEmitterArray(Model model, Shader shader, mat4 mod, vector<Emitter>& sel, Serial& serial, bool debug = false) {
		vec3 pos;
		FpgaProtocol prot = FpgaProtocol();
		Matematica mata = Matematica();
		string phasor = "mat = '{\n";

		int i = 0;

		AssignPhasesToArray(GetParticle(0));

		for (Emitter emi : emitters) {

			//Drawing the Emitters
			pos = emi.getPos();

			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);

			translation = translate(translation, pos);
			scaling = scale(scaling, vec3(0.05, 0.05, 0.05));

			shader.setVec3("color", emi.getCol());
			shader.setMat4("model", mod * translation * scaling);
			shader.use();

			if (sel.size() > 0 && emi == sel)
				shader.setVec3("color", vec3(0, 0, 1));

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
	void DrawSliceArray(Model model, Shader shader, mat4 mod) {
		Simulation sim;
		Slice sli = slices.at(0);
		int p = 0;

		for (Vox vox : sli.getVoxels()) {
			vox.setPres(sim.CalculatePressure(emitters, vox));
			sli.compareValues(vox.getPres());
			sli.setVoxels(vox, p);
			p++;
		}

		for (Vox vox : sli.getVoxels()) {
			mat4 translation = mat4(1.0f);
			mat4 scaling = mat4(1.0f);

			translation = translate(translation, vox.getPos());
			scaling = scale(scaling, vox.getSiz());

			vox.setCol(vox.ConvertToCol(vox.getPres(), sli.getMin(), sli.getMax()));
			shader.setMat4("model", mod * translation * scaling);
			shader.setVec3("color", vox.getCol());
			model.Draw(shader);
		}
	}

	void SetParticlePos(int n, vec3 pos) {
		particles.at(n).setPos(pos);
	}
	void SetEmitterPins() {
		ifstream file("EchoFeather.pin");
		int cnt = 0;

		string line;
		while (getline(file, line)) {
			/*
			if (line.find("data[") != string::npos) { // is a working data pin
				string token = line.substr(31);
				string pin = "";
				string loc = "";

				for (int i = 0; i < line.length(); i++) {
					if (token.at(i) == ' ') break;
					pin += token.at(i);
				}

				token = line.substr(5);
				for (int i = 0; i < line.length(); i++) {
					if (token.at(i) == ' ') break;
					loc += token.at(i);
				}

				emitters.at(stoi(loc)).setPin(stoi(pin));
				//emitters.at(stoi(loc)).setPin(-1);
			cnt++;
			}
			*/

			int a = stoi(line.substr(4, line.length()));
			emitters.at(cnt).setPin(a);
			cnt++;
		}

		file.close();
	}
	void SelectedEmiInfo(Emitter sel) {
		int i = GetEmiPointer(sel.getPos());

		cout << "\n" << "Selected emitter: " << i << "\n";
		emitters.at(i).DebugInfo();
	}
	void SetShadowPos(int n, vec3 pos) {
		particles.at(n).setPosShad(pos);
	}
	void GenerateAssignmentList() {
		string total = "";
		for (int i = 0; i < emitters.size(); i++) {
			total += "PIN_" + to_string(emitters.at(i).getPin()) + "\n";
		}

		ofstream file("assignments.txt");
		file << total;

		file.close();
	}
	void AssignPhasesToArray(Particle par) {
		vec3 pos = par.getPos();
		Simulation sim;
		Matematica mata;

		for (Emitter& emi : emitters) {
			emi.setPhase(sim.CalculatePhase(emi, par, getCm()));
			if (emi.getPhase() > emiPhMax)
				emiPhMax = emi.getPhase();
			if (emi.getPhase() < emiPhMin)
				emiPhMin = emi.getPhase();
		}
		for (Emitter& emi : emitters)
			emi.setCol(mata.mapFloat(emi.getPhase(), emiPhMin, emiPhMax, 0, 1, 1, 0));
	}
	void PrintPhases() {
		int i = 0;
		FpgaProtocol prott = FpgaProtocol();

		cout << "\nmat = '{\n";
		for (Emitter& emi : emitters) {
			cout << prott.calcSen(emi);
			if (i < 63)
				cout << ", ";

			if (i % 8 == 0 && i != 0)
				cout << "\n";
			i++;
		}

		cout << "\n};" << endl;
	}

	int getColSize() {
		return (collumnSize * cm) + (space * cm * collumnSize);
	}
	int getRowSize() {
		return (rowSize * cm) + (space * cm * rowSize);
	}
	float getCm() {
		return cm;
	}

	Particle& GetParticle(int pointer) {
		return particles.at(pointer);
	}
	Emitter& GetEmitter(int pointer) {
		return emitters.at(pointer);
	}
	Emitter& GetEmitter(vec3 pos) {
		Emitter posEmi;
		float error = INT_MAX; // X & Z

		for (Emitter& emi : emitters) {
			float score = abs(pos.x - (emi.getPos().x)) + abs(pos.z - (emi.getPos().z));

			if (score < error) {
				error = score;
				posEmi = emi;
			}
		}

		return posEmi;
	}
	int& GetEmiPointer(vec3 pos) {
		Emitter posEmi;
		float error = INT_MAX; // X & Z

		int pointer = 0;
		for (int i = 0; i < emitters.size(); i++) {
			float score = abs(pos.x - (emitters.at(i).getPos().x)) + abs(pos.z - (emitters.at(i).getPos().z));

			if (score < error) {
				error = score;
				pointer = i;
			}
		}

		return pointer;
	}
	vector<Emitter>& GetEmitter() {
		return emitters;
	}
	Vox& GetVoxel(vec3 pos) {
		Vox vox = slices.at(0).getVoxels().at(0);
		float error = INT_MAX; // X & Z

		/*for (Vox& v : slices.at(0).getVoxels()) {
			float score = abs(pos.x - (v.getPos().x)) + abs(pos.y - (v.getPos().y)) + abs(pos.z - (v.getPos().z));

			if (score < error) {
				error = score;
				vox = v;
			}
		}*/

		return vox;
	}
	vector<int>& GetPhases() {
		vector<int> temp;

		for (Emitter& emi : emitters)
			temp.push_back(emi.getPhase());

		return temp;
	}
	vec3 FindArrayCenter(float height) {
		int centerCoord = collumnSize * (rowSize / 2) + (collumnSize / 2);
		vec3 pos = emitters.at(centerCoord).getPos();
		return vec3(pos.x, pos.y + height, pos.z);
	}
	vec3 FindArrayBegining() {
		return emitters.at(0).getPos();
	}
};