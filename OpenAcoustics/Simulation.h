#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "Particle.h"
#include "Emitter.h"
#include "Vox.h"
#include "Slice.h"
#include <complex>

#define pi 3.14159
#define e  2.71828
#define sound_s 343

/*
Formulas from below were discovered and described
by A. Marzo, T. Corkett and B. W. Drinkwater.
I thank them for their incredible contribution
to this project!

Cited as
A. Marzo, T. Corkett and B. W. Drinkwater, 
"Ultraino: An Open Phased-Array System for Narrowband Airborne Ultrasound Transmission,"
in IEEE Transactions on Ultrasonics, Ferroelectrics, and Frequency Control, vol. 65, no. 1, pp. 102-111, Jan. 2018, 
doi: 10.1109/TUFFC.2017.2769399
*/

class Simulation {
private:
	float angleVec(vec3 a, vec3 b) {
		a *= 10;
		b *= 10;
		vec3 a_norm = normalize(a);
		vec3 b_norm = normalize(b);
		return acos(dot(a_norm, b_norm));
	}
	float sinc(float x) {
		return sin(x) / x;
	}

public:
	float CalculatePressure(vector<Emitter>& emit, Vox& targ, bool debil = false) {
		float sum = 0.0f;

		float threshold = 10;
		for (Emitter et : emit) {
			float Po = 1;
			float A = 1;

			float omega = pi * 2 * 40000;
			float k = omega / sound_s;

			float a = 0.9;
			float angle = angleVec(et.getPos(), targ.getPos());
			float dm = (a / 2) * k * sin(angle);
			float d = distance(targ.getPos(), et.getPos()) / 10;
			float Df = sinc(dm);
			float fi = et.getPhase();

			complex<float> i(0, 1);
			float expon = cos(fi + k * d);

			float Pr = expon;
			sum += Pr;

			if (false) {
				cout << "Angle: " << angle << endl;
				cout << "k: " << k << endl;
				cout << "Dm: " << dm << endl;
				cout << "Distance: " << d << endl;
				cout << "Df (sinc Dm): " << Df << endl;
				cout << "Fi: " << fi << endl;
				cout << "Expon: " << expon << endl;
				cout << "Pressure: " << Pr << endl;
				cout << "DfD: " << Df / d << endl;
				cout << endl;
			}
		}


		return sum;
	}
	float CalculatePhase(Emitter& emi, Particle targ, float cm) {
		/*
			Attention future me,
			i have no idea if the code below actually
			generates a good set of phases and if they actually
			work in real system. This is the code from the video (Not Marzo's).
			I kept it cause it looks cool, but thinking about real world is you problem.

			Love, Anton.
			08.07.2023

			:(
			08.12.24
		*/

		Matematica m;
		float d = distance(emi.getPos(), targ.getPos()) / (cm * 100);
		float waveLength = sound_s / emi.getFrequency();
		float targetPhase = (1.0f - m.decPart(d / waveLength)) * 2.0f * pi;

		return targetPhase / pi;
	}
};