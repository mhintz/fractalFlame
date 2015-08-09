#pragma once

#include "ofMain.h"

struct Sample {
		ofVec2f position;
		ofColor color;
};

struct TransformFunction {
		ofColor transformColor = ofColor::white;
		ofVec2f transform(const ofVec2f &);
};

struct Sierp1 : public TransformFunction {
		ofColor transformColor = ofColor(0.0f, 123.0f, 200.0f);
		ofVec2f transform(const ofVec2f &);
};

struct Sierp2 : public TransformFunction {
		ofColor transformColor = ofColor(144.0f, 30.0f, 15.0f);
		ofVec2f transform(const ofVec2f &);
};

struct Sierp3 : public TransformFunction {
		ofColor transformColor = ofColor(120.0f, 10.0f, 210.0f);
		ofVec2f transform(const ofVec2f &);
};

class Transformer {
private:
		Sierp1 t0 = Sierp1();
		Sierp2 t1 = Sierp2();
		Sierp3 t2 = Sierp3();

public:
		Sample applyTransform(const ofVec2f & pos);
};

ofVec2f runVariation(const ofVec2f &);