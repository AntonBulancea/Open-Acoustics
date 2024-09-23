#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/*
From Simulation.h you know the drill
I have no idea how to serial access an FPGA
So for now we're writing into a file
Everything else sounds like a you problem

Good luck there

-Anton 23/09/24

*/

class SerialConnection {
private:
	ofstream out;

public:
	SerialConnection() {

	}

	void begin() {
		out.open("phases.txt");
	}
	void close() {
		out.close();
	}
	void SendPhase(int &phase) {
		out.clear();
		out << phase << endl;
	}
};