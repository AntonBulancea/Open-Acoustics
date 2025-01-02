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
	int resolution = 5000; //How many clock cycles for a wave (0; 2pi)
	int freq = 40000;
	vector<int> sen;
	string promp = " ";

	int checkSuitability(int x, vector<int> s) {
		if (find(s.begin(), s.end(), x) != s.end())
			return checkSuitability(x - 1, s);
		else
			return x;
	}
public:
	int calcSen(Emitter emi) {
		Matematica m;
		int a = (int)m.mapFloat(emi.getPhase(), 0, pi_c * 2, 0, resolution);
		int s = checkSuitability(a, sen);

		promp += to_string(s) + " ";
		sen.push_back(s);

		return s;
	}

	string getSen() {
		return promp;
	}
};