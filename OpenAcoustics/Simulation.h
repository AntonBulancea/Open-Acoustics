#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "Particle.h"
#include "Emitter.h"


using namespace glm;

class Simulation {
private:
	float speedOfSound = 343000;
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
	
	float decPart(float a) {
		return a - ((int)a);
	}
	float pi() {
		return  3.14159274101257324219f;
	}
};