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
	void setOrigin(vec3 origin) {
		this->origin = origin;
	}
};