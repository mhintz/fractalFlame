#pragma once

#include "ofMain.h"

class Variation {
public:

	ofVec2f apply(const ofVec2f &);

private:

	float weight;
};