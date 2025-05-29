#pragma once

#include <iostream>
#include <vector>

#include "Emitter.h"
#include "Matematica.h"
#include "Simulation.h"
#include "Serial.h"

using namespace std;
class FpgaProtocol {
private:
	int resolution = 1250; //How many clock cycles for a wave (0; 2pi)
	int freq = 40000;
	vector<int> sen;
	string promp = " ";

	float biggest = 0;
	int checkSuitability(int x, vector<int> s) {
		if (find(s.begin(), s.end(), x) != s.end())
			return checkSuitability(x - 1, s);
		else
			return x;
	}
public:
	int calcSen(Emitter emi) {
		int c = static_cast<int>(std::round(emi.getPhase()/(2*pi_c) * resolution));
		return c;
	}

	int error() {
		return biggest;
	}
	string getSen() {
		return promp;
	}
};