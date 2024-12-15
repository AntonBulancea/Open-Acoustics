#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Matematica.h"

using namespace std;
using namespace glm;

class Vox {
private:
	vec3 pos;
	vec3 col;
	float size;
	float pres;
public:

	Vox(vec3 pos, vec3 col, float size, float pres){
		this->pos = pos;
		this->col = col;
		this->pres = pres;
		this->size = size;
	}

	vec3 getPos() {
		return pos;
	}
	vec3 getCol() {
		return col;
	}
	vec3 getSiz() {
		return vec3(size,size,size);
	}
	float getPres() {
		return pres;
	}

	vec3 ConvertToCol(float press, float min, float max) {
		Matematica mata;
		float color = mata.invert(mata.mapFloat(press,min,max,0,1),1);

		/*if (color < 0.8)
			color = 0;
		else
			color = 1;*/

		vec3 col = vec3(color, color, color);
		return col;
	}

	void setPos(vec3 pos) {
		this->pos = pos;
	}
	void setCol(vec3 col) {
		this->col = col;
	}
	void setPres(float pres) {
		this->pres = pres;
	}
};