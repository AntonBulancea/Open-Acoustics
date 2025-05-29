#pragma once

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class FilingSystem {
public:
	string fileName;
	bool readOnly;

	FilingSystem(string name) {
		fileName = name;
	}

	void write(string text) {
		if (readOnly)
			return;

		ofstream file(fileName);
		file << text;

		file.close();
	}
	string read() {
		string s = "aa";

		ifstream file(fileName);
		file >> s;

		file.close();
		return s;
	}
};