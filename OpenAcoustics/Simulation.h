#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "Particle.h"
#include "Emitter.h"
#include "Vox.h"
#include "Slice.h"

using namespace glm;

class Simulation {
private:
	float speedOfSound = 343000;
	float excitationVoltage = 1;
	
	float decPart(float a) {
		return a - ((int)a);
	}
	float pi() {
		return  3.14159274101257324219f;
	}
	float e() {
		return 2.71828;
	}
	float sinc(float x) {
		if (x == 0.0f) {
			return 1.0f;
		}
		else {
			return sin(x) / x;
		}
	}
	float angleBetweenVectors(vec3 a, vec3 b) {
		float dotProduct = dot(a, b);
		float magnitudeA = length(a);
		float magnitudeB = length(b);
		float cosTheta = dotProduct / (magnitudeA * magnitudeB);
		cosTheta = clamp(cosTheta, -1.0f, 1.0f);
		float angleRadians = std::acos(cosTheta);
		return angleRadians;
	}

public:
	float CalculatePhase(Emitter &emi, Particle &par) {

		/*
			Attention future me,
			i have no idea if the code below actually
			generates a good set of phases and if they actually
			work in real system. This is the code from the video (Not Marzo's).
			I kept it cause it looks cool, but thinking about real world is you problem.

			Love, Anton.
			08.07.2023
		*/


		vec3 emiPos = emi.getPos();
		vec3 parPos = par.getPos();

		float distance = glm::distance(emiPos,parPos);
		float waveLenght = emi.getFrequency() / speedOfSound * 2*pi();

		float r = length(distance);
		float s = r * waveLenght;
		
		float targetPhase = fmodf(s,2*pi()) + 2*pi();
		return targetPhase - 6;
	}
	float CalculatePressure(vector<Emitter>& emis, Vox vox) {
		float sumPres = 1;
		for (Emitter emi : emis) {
			float Pr;

			float Po = emi.getAmplitude();
			float d = distance(vox.getPos(), emi.getPos());
			float A = 2 * excitationVoltage;
			float k = emi.getFrequency() / speedOfSound * 2 * pi();
			float a = emi.getRadius();
			float ph = emi.getPhase();
			float th = angleBetweenVectors(emi.getPos(), vox.getPos());
			float i = 1; //???

			float Df = sinc(k * a * sin(th));

			Pr = (Po * A * Df * pow(e(), (i * (ph + k * d)))) / d;
			sumPres += Pr;
		}
		return sumPres;
	}
};