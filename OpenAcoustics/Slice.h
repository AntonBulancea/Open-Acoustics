#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Slice {
private:
	vector<float> pressure;
	vector<vec3> colorMap;
public:
	void setPressure(float pres) {
		pressure.push_back(pres);
	}
	void setPressure(vector<float> &pressure) {
		this->pressure = pressure;
	}

	void clearPressure() {
		pressure.clear();
	}
	void clearColor() {
		colorMap.clear();
	}

	float getPressure(int num) {
		return pressure.at(num);
	}
	vec3 getColor(int num) {
		return colorMap.at(num);
	}
};	