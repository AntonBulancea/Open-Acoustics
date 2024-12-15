#pragma once
class Matematica {
public:
	float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
		//return x;
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
	float mapFloat(float x, float in_min, float in_max, float out_min, float out_max, float ceiling, float floor) {
		float res = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		if (res > ceiling)
			res = ceiling;
		else if (res < floor)
			res = floor;
		//return x;
		return res;
	}
	float invert(float x, float m) {
		return m - x;
	}
	float decPart(float a) {
		return a - ((int)a);
	}
};