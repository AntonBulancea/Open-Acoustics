#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	vec3 ConvertToCol(float press) {
		press = 1 / press;
		press *= 2000;

		vec3 col = vec3(press, press, press);
		//this->col = col;
		//cout << col.x << endl;
		return col;
	}
	void SetToCol(float freq) {
		col = ConvertToCol(freq);
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