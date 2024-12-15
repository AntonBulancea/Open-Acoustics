#pragma once

#include <iostream>
#include <vector>

#include "Vox.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Slice {
private:
	vector<Vox> voxels;
	vec3 origin;

	float minPr = 100000, maxPr = 0;
public:
	vector<Vox> getVoxels() {
		return voxels;
	}
	vec3 getOrigin() {
		return origin;
	}

	void setVoxels(vector<Vox> voxels) {
		this->voxels = voxels;
	}
	void setVoxels(Vox voxel) {
		voxels.push_back(voxel);
	}
	void setVoxels(Vox voxel, int pointer) {
		voxels.at(pointer) = voxel;
	}
	void setOrigin(vec3 origin) {
		this->origin = origin;
	}
	void compareValues(float val) {
		if (val > maxPr)
			maxPr = val;
		else if (val < minPr)
			minPr = val;
	}

	float getMax() {
		return maxPr;
	}
	float getMin() {
		return minPr;
	}
};